/*
  LEDPong v1.1.0
  ==============

  Ein modernes Pong-Spiel auf einem 1 m langen adressierbaren LED-Strip
  mit Arcade-Tastern, Sound, Sprachansagen und fünf Betriebsarten.

  Copyright (C) 2026 Andreas Fatum

  Dieses Programm basiert auf:

  1. "1D Pong" von FlyingAngel (2020)
     https://www.hackster.io/flyingangel/1d-pong-85e965
     Die dort veröffentlichte Originalfassung steht unter der
     GNU General Public License, Version 3 oder später.

  2. "Pong mit Arduino" von Armin Duft (DH1DA)
     https://e-duft.de/pong-mit-arduino/
     Sound-erweiterte Fassung; Weiterverwendung und Veröffentlichung
     mit freundlicher Genehmigung von Armin Duft.

  Wesentliche LEDPong-Erweiterungen:

    - Unterstützung eines 1-m-WS2812B-Strips mit 144 LEDs
    - auf 144 LEDs skaliertes virtuelles 60-LED-Spielmodell
    - fünf Betriebsarten: Normal, Pro, Insane, Rallye und Training
    - 5-fach-Drehwahlschalter
    - Lautstärkeregelung über 10-kΩ-Potentiometer 
	- Ein-/Ausschalter für den Lautsprecher
    - stabilisierte DFPlayer-Mini-Ansteuerung
    - Sprachansagen und Soundeffekte
    - dynamisch schrumpfende Rückschlagzonen im Rallye-Modus
    - überarbeitete Pinbelegung und Hardware
    - USB-C-Serviceanschluss zum Flashen des Arduino Nano
    - 3D-gedruckte Gehäuse- und Bedienelemente

  Lizenz
  -------

  Dieses Programm ist freie Software: Sie können es unter den Bedingungen
  der GNU General Public License, wie von der Free Software Foundation
  veröffentlicht, weitergeben und/oder modifizieren; entweder gemäß
  Version 3 der Lizenz oder – nach Ihrer Wahl – jeder späteren Version.

  Dieses Programm wird in der Hoffnung bereitgestellt, dass es nützlich ist,
  jedoch OHNE JEDE GEWÄHRLEISTUNG; sogar ohne die implizite Gewährleistung
  der MARKTREIFE oder der EIGNUNG FÜR EINEN BESTIMMTEN ZWECK. Weitere
  Einzelheiten finden Sie in der GNU General Public License.

  Zusammen mit diesem Programm sollte eine Kopie der GNU General Public
  License bereitgestellt werden. Siehe <https://www.gnu.org/licenses/>.

  SPDX-License-Identifier: GPL-3.0-or-later
*/

#define FASTLED_INTERNAL           	// Disable version number message in FastLED library (looks like an error)
#include <FastLED.h>
#include "Arduino.h"
#include "SoftwareSerial.h"         // control mp3 Player via serial interface with 9600 baud
#include "DFRobotDFPlayerMini.h"    // mp3 Player

// ============================================================================
// Spielmodi und Spielparameter
// ============================================================================

enum GameMode {
  MODE_NORMAL = 0,
  MODE_PRO = 1,
  MODE_INSANE = 2,
  MODE_RALLYE = 3,
  MODE_TRAINING = 4,
  MODE_INVALID = 255
};

struct PongModeConfig {
  int startSpeed;
  int speedStep;
  int fastestSpeed;
  int boost0;
  int boost1;
  int boost2;
  unsigned char endZoneBase;
};

// Explicit prototypes are required in a single-file Arduino sketch because
// Arduino's automatic prototype generator can otherwise place declarations
// using GameMode/PongModeConfig before these custom types.
int modeAnnouncementTrack(GameMode mode);
PongModeConfig pongModeConfigFor(GameMode mode);

int modeAnnouncementTrack(GameMode mode) {
  return mode >= MODE_NORMAL && mode <= MODE_TRAINING ? 20 + static_cast<int>(mode) : 0;
}

PongModeConfig pongModeConfigFor(GameMode mode) {
  // start, step, limit, boost0, boost1, boost2, end zone size at 60 LEDs/m
  switch (mode) {
    case MODE_PRO:
      return {40, 2, 5, 15, 10, 5, 6};
    case MODE_INSANE:
      return {30, 3, 3, 10, 7, 3, 4};
    case MODE_RALLYE:
      // Pro settings with the Insane maximum speed.
      return {40, 2, 3, 15, 10, 5, 6};
    case MODE_TRAINING:
      return {40, 2, 5, 15, 10, 5, 6};
    case MODE_NORMAL:
    default:
      return {50, 1, 7, 20, 10, 5, 7};
  }
}

#define BASE_NUM_LEDS 60       		// Original game reference: 60 LEDs per metre
#define NUM_LEDS 144            		// Installed LEDs on the 1 m strip
#define PLAYER0  2              		// Button player 0
#define PLAYER1  3              		// button player 1
#define rxPin    4              		// mp3 serial interface RXD
#define txPin    5              		// mp3 serial interface TXD
#define DATA_PIN 6               		// LED data pin
#define MODE1    7                      // Pin for game mode 1
#define MODE2    8                      // Pin for game mode 2
#define MODE3    9                      // Pin for game mode 3
#define MODE4   10                      // Pin for game mode 4
#define MODE5   11                      // Pin for game mode 5
#define VOLUME_PIN A1             		// 10k potentiometer wiper for volume

const byte MODE_COUNT = 5;
const byte modePins[MODE_COUNT] = {MODE1, MODE2, MODE3, MODE4, MODE5};
const unsigned long MODE_DEBOUNCE_MS = 60;
const byte RALLYE_RETURNS_PER_SHRINK = 3;
const byte RALLYE_MIN_END_ZONE_BASE = 1;
const byte RALLYE_SHRINK_STEP_BASE = 1;
const byte RALLYE_FLASH_COUNT = 2;
const byte RALLYE_FLASH_ON_MS = 90;
const byte RALLYE_FLASH_OFF_MS = 70;
const int TRAINING_ARDUINO_PLAYER = 0;
const int TRAINING_HUMAN_PLAYER = 1;
const byte TRAINING_MISS_INTERVAL = 10;
const unsigned long TRAINING_AUTOMATIC_SERVE_DELAY_MS = 750UL;
const byte VOLUME_MIN = 1;
const byte VOLUME_MAX = 20;
const unsigned long START_TRACK_TIMEOUT_MS = 30000UL;

SoftwareSerial mySerial(rxPin,txPin);   // RX, TX

byte nfiles = 24;                       // highest mp3 file number currently used  
byte ifile  = 1;                        // number of file played next  
int brightness       =     105; 		    // default start brightness (war: 155)
byte playerBtnPin[] =   {PLAYER0, PLAYER1}; // Pins for buttons
byte volumeLevel    =       10; 		    // current DFPlayer volume 0 ... 30
int gameSpeedMin    =       50; 		    // min game-speed
int gameSpeedMax    =        5; 		    // max game-speed
int gameSpeedStep   =        1; 		    // increase speed when change direction
int ballSpeedMax    =        7; 		    // max ball-speed  (1 is maximum, larger values are slower)
int ballBoost0      =       20; 		    // superboost last position
int ballBoost1      =       10; 		    // boost second last position
int ballBoost2      =       5;  		    // boost third last position
byte playerColor[]  =  {0, 96}; 		    // red & green
int winRounds       =        5; 		    // number of points for winning the game
int endZoneSize     = (7L * NUM_LEDS + BASE_NUM_LEDS / 2) / BASE_NUM_LEDS; // set by applyModeSettings()
int endZoneColor    =      160; 		    // color endzone

// Definition System-Variables
boolean activeGame = false;             // true when game is active
boolean brightSet = true;               // set brightness at start of the game
unsigned long previousMoveMicros;       // timestamp of last ball move (microseconds)
unsigned long previousButtonMillis;     // timestamp of last button-press
unsigned long previousVolumeMillis;     // timestamp of last volume check
byte pendingVolumeLevel = 255;          // Mittelwertberechnung
byte pendingVolumeCount = 0;            // Mittelwertberechnung
byte previousVolumeLevel = 255;         // force initial volume update
int playerButtonPressed[2];             // ball-position where button was pressed; „-1“ button not pressed
int previousButtonPos = -1;             // position of last button-press
byte previousButtonColor;               // color of field for last Button-press
int playerScore[2];                     // current Score
byte playerStart;                       // who starts game
int gameSpeed;                          // current game-speed
int ballDir = 1;                        // direction, ball is moving (+/- 1)
int ballPos;                            // ball-position
int ballSpeed;                          // ball-speed (higher = slower)
int rallyeStartEndZoneSize;             // full end-zone size restored after each point
int rallyeMinimumEndZoneSize;           // smallest allowed Rallye end zone
int rallyeShrinkStep;                   // LEDs removed at each Rallye stage
int rallyeSuccessfulReturns = 0;        // successful returns in the current rally
int trainingHitPosition = -1;           // Arduino return position; -1 deliberately misses
GameMode selectedMode = MODE_NORMAL;
GameMode modeCandidate = MODE_INVALID;
unsigned long modeCandidateSince = 0;

CRGB leds[NUM_LEDS];                          // Define the array of LEDs
byte previousButtonBright = brightness / 2;   // bright of marked last position when button pressed
byte scoreDimBright       = brightness / 4;   // bright of dimmed score

// Explicit forward declarations.
// Arduino's automatic prototype generator may skip functions with a default argument.
void updateVolume(boolean forceUpdate = false);
GameMode readModeSwitchRaw();
void updateModeSelection();
void applyModeSettings(GameMode mode);
void announceMode(GameMode mode);
void clearDfPlayerInput();
boolean waitForTrackFinished(unsigned long timeoutMs);
boolean playTrackAndWait(byte folder, byte file, unsigned long timeoutMs);
void startSelectedMode();
bool trainingAcceptsButton(GameMode mode, int player);
bool trainingNeedsAutomaticReturn(GameMode mode, int ballDirection);
bool trainingServeIsAutomatic(GameMode mode, int player);
boolean buttonBounce(byte button, byte bounceTime);
void game();
void InitializePlayers();
void GameLoop();
void CheckButtons();
void CheckButtonPressedPosition();
void ChangeDirection();
int chooseTrainingHitPosition();
void prepareTrainingReturn();
void performTrainingReturn();
void resetRallyeRound();
void updateRallyeAfterReturn();
void flashRallyeShrink(int oldZoneSize);
void CheckScore();
void CheckWinner();
void GeneratePlayField(byte bright);
void GenerateEndZone();
void GenerateScore(int bright);
void GenerateLastHit();
void BlinkNewScore(int pos, byte color);
void winRainbow(boolean won);
void Rainbow();
void dfpExecute(byte CMD, byte Par1, byte Par2);

//---------------
void setup() {
  Serial.begin(115200);                 // for Debug
  mySerial.begin(9600);                 // for mp3 Player
  delay(500);
  Serial.println();
  Serial.println(F("1D PONG GAME"));

  dfpExecute(0x3F, 0x00, 0x00);       	// Send request to mp3 for initialization parameters
  while (mySerial.available() < 10) {    // Wait until initialization parameters are received
    delay(30);
  }

  delay(300);                            // Let the DFPlayer finish its internal startup
  clearDfPlayerInput();
  updateVolume(true);                    // Set initial potentiometer volume reliably
  Serial.println("Setup Init OK");

  randomSeed(analogRead(0));          	// better random start
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);    // FastLed definition

  pinMode(playerBtnPin[0], INPUT_PULLUP); // PINs for buttons
  pinMode(playerBtnPin[1], INPUT_PULLUP);

  for (byte i = 0; i < MODE_COUNT; i++) {
    pinMode(modePins[i], INPUT_PULLUP);
  }

  delay(20);
  GameMode initialMode = readModeSwitchRaw();
  if (initialMode != MODE_INVALID) {
    selectedMode = initialMode;
  }
  modeCandidate = selectedMode;
  modeCandidateSince = millis();
  applyModeSettings(selectedMode);

  playerStart = random(2);                // random starting player
  Serial.println(playerStart);
  Serial.println(" Spieler beginnt");
  Rainbow();                              // color effect

  // Startup order is deliberately strict: greeting, instructions, selected mode.
  // Clear any old DFPlayer status frame before the first spoken file.
  updateVolume(true);
  clearDfPlayerInput();
  delay(150);

  playTrackAndWait(0x01, 10, START_TRACK_TIMEOUT_MS);  // 010.mp3: greeting 
  delay(150);
  clearDfPlayerInput();

  playTrackAndWait(0x01, 11, START_TRACK_TIMEOUT_MS);  // 011.mp3: brightness instruction
  delay(150);
  clearDfPlayerInput();

  playTrackAndWait(0x01, 12, START_TRACK_TIMEOUT_MS);  // 012.mp3: start instruction
  delay(150);
  clearDfPlayerInput();

  announceMode(selectedMode);             // 020..024 only after 012 has finished
}

// *********************************
void loop() {
  updateVolume();                         // allow volume adjustment while idle
  updateModeSelection();                  // accept a stable new rotary-switch position

  if (brightSet) {
    GeneratePlayField(brightness);        // show play-field
    FastLED.show();

    // set brightness (button player 0)
    if (buttonBounce(0, 5) == true) {
      brightness += 50;
      if (brightness > 255) {
        brightness = 55;
      }
      Serial.print(F("brightness "));
      Serial.println(brightness);
      GeneratePlayField(brightness);
      FastLED.show();

      while (buttonBounce(0, 5) == true) {}   // wait for button release
    }
  }

  if (buttonBounce(1, 5) == true) {       // confirm/start selected mode
    while (buttonBounce(1, 5) == true) {} // wait for button release
    startSelectedMode();
  }
}

//---------------
GameMode readModeSwitchRaw() {
  byte activeContacts = 0;
  GameMode detectedMode = MODE_INVALID;

  for (byte i = 0; i < MODE_COUNT; i++) {
    if (digitalRead(modePins[i]) == LOW) {
      activeContacts++;
      detectedMode = (GameMode)i;
    }
  }

  // During switching, a rotary switch may briefly connect none or two contacts.
  // Keep the previous mode until exactly one valid contact is active.
  return activeContacts == 1 ? detectedMode : MODE_INVALID;
}

//---------------
void updateModeSelection() {
  GameMode rawMode = readModeSwitchRaw();

  if (rawMode == MODE_INVALID) {
    modeCandidate = MODE_INVALID;
    return;
  }

  if (rawMode != modeCandidate) {
    modeCandidate = rawMode;
    modeCandidateSince = millis();
    return;
  }

  if (rawMode != selectedMode && millis() - modeCandidateSince >= MODE_DEBOUNCE_MS) {
    selectedMode = rawMode;
    applyModeSettings(selectedMode);
    if (selectedMode == MODE_TRAINING) {
      playerStart = random(2);             // random first serve when entering Training
    }
    memset(playerScore, 0, sizeof(playerScore));
    previousButtonPos = -1;
    activeGame = false;
    brightSet = true;

    FastLED.clear();
    FastLED.show();

    // Stop any previous announcement/special-mode sound first. Some DFPlayer
    // clones otherwise swallow the immediately following play command.
    dfpExecute(0x16, 0x00, 0x00);
    delay(150);
    clearDfPlayerInput();
    announceMode(selectedMode);
  }
}

//---------------
void applyModeSettings(GameMode mode) {
  // Values remain expressed in the original 60-LED reference units and are
  // scaled to the installed 144-LED strip below.
  PongModeConfig config = pongModeConfigFor(mode);
  gameSpeedMin = config.startSpeed;
  gameSpeedStep = config.speedStep;
  ballSpeedMax = config.fastestSpeed;
  ballBoost0 = config.boost0;
  ballBoost1 = config.boost1;
  ballBoost2 = config.boost2;
  endZoneSize = ((long)config.endZoneBase * NUM_LEDS + BASE_NUM_LEDS / 2) / BASE_NUM_LEDS;
  rallyeStartEndZoneSize = endZoneSize;
  rallyeMinimumEndZoneSize =
      ((long)RALLYE_MIN_END_ZONE_BASE * NUM_LEDS + BASE_NUM_LEDS / 2) / BASE_NUM_LEDS;
  rallyeShrinkStep =
      ((long)RALLYE_SHRINK_STEP_BASE * NUM_LEDS + BASE_NUM_LEDS / 2) / BASE_NUM_LEDS;
  rallyeSuccessfulReturns = 0;

  Serial.print(F("mode "));
  Serial.print((byte)mode + 1);
  Serial.print(F(", startSpeed "));
  Serial.print(gameSpeedMin);
  Serial.print(F(", endZone LEDs "));
  Serial.println(endZoneSize);
}

//---------------
void clearDfPlayerInput() {
  while (mySerial.available() > 0) {
    mySerial.read();
  }
}

//---------------
// Wait for the DFPlayer's unsolicited "track finished" frame.
// Command 0x3D is TF-card playback finished; 0x3C is USB playback finished.
// Volume changes remain possible while waiting. A timeout prevents lockups if
// a clone does not emit the completion frame.
boolean waitForTrackFinished(unsigned long timeoutMs) {
  byte frame[10];
  byte frameIndex = 0;
  unsigned long startedAt = millis();

  while ((unsigned long)(millis() - startedAt) < timeoutMs) {
    updateVolume();

    while (mySerial.available() > 0) {
      byte value = mySerial.read();

      // Re-synchronise strictly on the DFPlayer start byte.
      if (frameIndex == 0) {
        if (value != 0x7E) {
          continue;
        }
        frame[frameIndex++] = value;
        continue;
      }

      // A new start byte inside an incomplete frame begins a fresh frame.
      if (value == 0x7E) {
        frame[0] = value;
        frameIndex = 1;
        continue;
      }

      frame[frameIndex++] = value;

      if (frameIndex == sizeof(frame)) {
        frameIndex = 0;

        uint16_t receivedChecksum = ((uint16_t)frame[7] << 8) | frame[8];
        uint16_t calculatedChecksum = -(frame[1] + frame[2] + frame[3] +
                                        frame[4] + frame[5] + frame[6]);

        boolean validFrame = frame[0] == 0x7E &&
                             frame[1] == 0xFF &&
                             frame[2] == 0x06 &&
                             frame[9] == 0xEF &&
                             receivedChecksum == calculatedChecksum;

        if (validFrame && (frame[3] == 0x3D || frame[3] == 0x3C)) {
          return true;
        }
      }
    }

    delay(5);
  }

  Serial.println(F("DFPlayer: track-finished timeout"));
  return false;
}

//---------------
boolean playTrackAndWait(byte folder, byte file, unsigned long timeoutMs) {
  clearDfPlayerInput();
  dfpExecute(0x0F, folder, file);
  return waitForTrackFinished(timeoutMs);
}

//---------------
void announceMode(GameMode mode) {
  int track = modeAnnouncementTrack(mode);
  if (track > 0) {
    dfpExecute(0x0F, 0x01, track);
  }
}

//---------------
void startSelectedMode() {
  applyModeSettings(selectedMode);
  brightSet = false;

  activeGame = true;
  dfpExecute(0x0F, 0x01, 1);            // game start sound
  game();

  activeGame = false;
  brightSet = true;
}


//---------------

//---------------

//---------------

//---------------


//---------------
// Read a 10k potentiometer on A1 and set DFPlayer volume (1 ... 20).
// The value is checked only every 200 ms and transmitted only when the
// resulting volume step changes, preventing unnecessary serial commands.
void updateVolume(boolean forceUpdate) {
  if (!forceUpdate && millis() - previousVolumeMillis < 200) {
    return;
  }

  previousVolumeMillis = millis();

  // Five readings reduce ADC noise.
  long rawSum = 0;

  for (byte i = 0; i < 5; i++) {
    rawSum += analogRead(VOLUME_PIN);
    delayMicroseconds(200);
  }

  int rawValue = rawSum / 5;
  byte newVolume = map(rawValue, 0, 1023, VOLUME_MIN, VOLUME_MAX);

  // Set the initial volume immediately.
  if (forceUpdate) {
    volumeLevel = newVolume;
    previousVolumeLevel = newVolume;
    pendingVolumeLevel = newVolume;
    pendingVolumeCount = 0;

    dfpExecute(0x06, 0x00, volumeLevel);

    Serial.print(F("volume "));
    Serial.println(volumeLevel);
    return;
  }

  // Nothing changed.
  if (newVolume == previousVolumeLevel) {
    pendingVolumeLevel = newVolume;
    pendingVolumeCount = 0;
    return;
  }

  // First occurrence of a possible new value.
  if (newVolume != pendingVolumeLevel) {
    pendingVolumeLevel = newVolume;
    pendingVolumeCount = 1;
    return;
  }

  // Accept only after two consecutive identical measurements.
  pendingVolumeCount++;

  if (pendingVolumeCount >= 2) {
    volumeLevel = newVolume;
    previousVolumeLevel = newVolume;
    pendingVolumeCount = 0;

    dfpExecute(0x06, 0x00, volumeLevel);

    Serial.print(F("volume "));
    Serial.println(volumeLevel);
  }
}


//---------------
// function to debounce button (true == pressed, false == not pressed)
boolean buttonBounce(byte button, byte bounceTime) {       
  boolean result = false;
  if (digitalRead(playerBtnPin[button]) == LOW) {
    delay (bounceTime);
    if (digitalRead(playerBtnPin[button]) == LOW) {
      result = true;
    }
  }
  return result;
}

// ============================================================================
// Modus-Hilfsfunktionen: Training und Rallye
// ============================================================================

bool trainingAcceptsButton(GameMode mode, int player) {
  return mode != MODE_TRAINING || player == TRAINING_HUMAN_PLAYER;
}

bool trainingNeedsAutomaticReturn(GameMode mode, int ballDirection) {
  return mode == MODE_TRAINING && ballDirection == -1;
}

bool trainingServeIsAutomatic(GameMode mode, int player) {
  return mode == MODE_TRAINING && player == TRAINING_ARDUINO_PLAYER;
}

bool trainingShouldMiss(long randomRoll, int missInterval) {
  return missInterval > 0 && randomRoll == 0;
}

int rallyeZoneSize(int startZoneSize,
                          int minimumZoneSize,
                          int shrinkStep,
                          int returnsPerShrink,
                          int successfulReturns) {
  if (returnsPerShrink <= 0 || shrinkStep <= 0 || successfulReturns <= 0) {
    return startZoneSize;
  }

  int shrinkStages = successfulReturns / returnsPerShrink;
  int zoneSize = startZoneSize - shrinkStages * shrinkStep;
  return zoneSize < minimumZoneSize ? minimumZoneSize : zoneSize;
}

bool rallyeShouldFlash(int successfulReturns,
                              int returnsPerShrink,
                              int currentZoneSize,
                              int minimumZoneSize) {
  return returnsPerShrink > 0 &&
         successfulReturns > 0 &&
         successfulReturns % returnsPerShrink == 0 &&
         currentZoneSize > minimumZoneSize;
}

//---------------
void game() {
  while (activeGame)   {
    GameMode requestedMode = readModeSwitchRaw();
    if (requestedMode != MODE_INVALID && requestedMode != selectedMode) {
      activeGame = false;                         // apply rotary-switch change after leaving game()
      break;
    }

    gameSpeed = gameSpeedMin;                     // set starting game speed
    ballSpeed = gameSpeed;                        // set starting ball speed
    memset(playerButtonPressed, -1, sizeof(playerButtonPressed));   // clear keypress
    resetRallyeRound();                            // every point starts with the full end zone
    trainingHitPosition = -1;

    GeneratePlayField(scoreDimBright);            // show gamefield with dimmed score
    FastLED.show();
    delay(1000);
    InitializePlayers();          // set the player-settings -> wait for keypress to start game
    GameLoop();                   // main loop: move ball -> ball left gamefield? -> check keypress -> check if keypress in endzone -> change direction
    resetRallyeRound();           // restore full zones before displaying the new score
    CheckScore();                 // check who made score and show it
    CheckWinner();                // check if we have a winner
  }
}

//---------------
void InitializePlayers() {

  if (playerStart == 0) {                             // initialize for player 0
    ballDir = 1;                                      // set ball direction
    ballPos = 0;                                      // set startposition of ball

    leds[0] = CHSV(playerColor[0], 255, brightness);  // first LED für Spieler 0
    FastLED.show();
    if (trainingServeIsAutomatic(selectedMode, TRAINING_ARDUINO_PLAYER)) {
      delay(TRAINING_AUTOMATIC_SERVE_DELAY_MS);       // Arduino serves automatically
    }
    else {
      while (digitalRead(playerBtnPin[0]) == HIGH) {} // wait for player 0 button (default red button unless changed)
    }
    leds[0] = CHSV(endZoneColor, 255, brightness);    // restore EndZone display
    FastLED.show();
  }
  else {                                              // initialize for player 1
    ballDir = -1;                                     // set ball direction
    ballPos = NUM_LEDS - 1;                           // set startposition of ball

    leds[NUM_LEDS-1] = CHSV(playerColor[1], 255, brightness);   // last LED für player 1
    FastLED.show();
    while (digitalRead(playerBtnPin[1]) == HIGH) {}   // wait for button
    leds[NUM_LEDS-1] = CHSV(endZoneColor, 255, brightness);    // restore EndZone display
    FastLED.show();
  }
  dfpExecute(0x0F,0x01,4);                            // Play mp3 launch-ball sound

  if (selectedMode == MODE_TRAINING) {
    prepareTrainingReturn();
    previousMoveMicros = micros();
  }
}

//------------------
void GameLoop() {                                     // loop, exit with break when one player made a score
  while (true) {
    // updateVolume();                                  // disabled during rallies: DFPlayer volume commands can disturb sound playback

    // ballSpeed remains expressed in the original 60-LED timing units.
    // Convert it to microseconds so the physical speed stays approximately
    // identical on a 1 m strip with a different LED density.
    const unsigned long moveIntervalMicros =
        (unsigned long)ballSpeed * 1000UL * BASE_NUM_LEDS / NUM_LEDS;
    const unsigned long currentMicros = micros();

    if ((unsigned long)(currentMicros - previousMoveMicros) >= moveIntervalMicros) {
      previousMoveMicros = currentMicros;
      GeneratePlayField(scoreDimBright);
      ballPos += ballDir;
      if (ballPos < 0 || ballPos >= NUM_LEDS) {       // ball left endzone?
        dfpExecute(0x0F,0x01,3);                      // Play mp3 buzz
        break;                                        // leave loop -> one player made a score
      }
      leds[ballPos] = CHSV(0, 0, brightness);         // generate ball (white)
      FastLED.show();

      if (trainingNeedsAutomaticReturn(selectedMode, ballDir) &&
          trainingHitPosition >= 0 && ballPos == trainingHitPosition) {
        performTrainingReturn();
      }
    }

    CheckButtons();     // check keypress

    // fix positions of keypress for debugging
    // if (ballPos == 3) playerButtonPressed[0] = 3;
    // if (ballPos == NUM_LEDS - 1) playerButtonPressed[1] = NUM_LEDS - 1;
    CheckButtonPressedPosition();
  }
}

//---
void CheckButtons() {                                 // check if buttons pressed
  for (int i = 0; i < 2; i++) {
    if (!trainingAcceptsButton(selectedMode, i)) {
      continue;
    }

                                                      // player pressed button?
    if (playerButtonPressed[i] == -1 && digitalRead(playerBtnPin[i]) == LOW && (ballDir + 1) / 2 == i) {
      // (ballDir + 1) / 2 == i  -->  TRUE, when:
      // ballDir == -1  AND  i = 0  -->  player 0 is active player
      // ballDir == +1  AND  i = 1  -->  player 1 is active player
      // only the button-press of the active player is stored                               
      dfpExecute(0x0F,0x01,2);                        //Play mp3 pong

      playerButtonPressed[i] = ballPos;               //store position of pressed button
      previousButtonPos = ballPos;
      previousButtonColor = playerColor[i];
      previousButtonMillis = millis();                // store time when button was pressed
    }
  }
}

// *** check, button pressed while ball in endzone, if so, change direction of ball
void CheckButtonPressedPosition() {
  if (ballDir == -1 && playerButtonPressed[0] <= endZoneSize - 1 && playerButtonPressed[0] != -1) {
    ChangeDirection();
  }

  if (ballDir == +1 && playerButtonPressed[1] >= NUM_LEDS - endZoneSize) {
    ChangeDirection();
  }
}

//---
void ChangeDirection() {
  ballDir *= -1;
  gameSpeed -= gameSpeedStep;
  if (gameSpeed > 30) {                               // at gamestart increase speed faster
    gameSpeed -= gameSpeedStep;
  }
  ballSpeed = gameSpeed;

  // Map the distance from either strip end back to the original 60-LED grid.
  // This keeps Super-Boost and both Boost zones approximately the same physical size.
  int distanceFromEnd = min(ballPos, NUM_LEDS - 1 - ballPos);
  int virtualDistance = (long)distanceFromEnd * BASE_NUM_LEDS / NUM_LEDS;

  if (virtualDistance == 0) {                         // original outermost LED zone
    ballSpeed -= ballBoost0;                          // Super-Boost
  }
  else if (virtualDistance == 1) {                    // original second LED zone
    ballSpeed -= ballBoost1;                          // Boost
  }
  else if (virtualDistance == 2) {                    // original third LED zone
    ballSpeed -= ballBoost2;                          // Boost
  }

  if (ballSpeed < 1) {                                // avoid overflow
    ballSpeed = 1;
  }
  ballSpeed = max(ballSpeed, ballSpeedMax);           // limit the maximum ballspeed - Result is the higher (=slower) value

  updateRallyeAfterReturn();
  prepareTrainingReturn();

  memset(playerButtonPressed, -1, sizeof(playerButtonPressed)); // clear keypress
}

//---
int chooseTrainingHitPosition() {
  if (trainingShouldMiss(random(TRAINING_MISS_INTERVAL), TRAINING_MISS_INTERVAL)) {
    return -1;
  }

  return random(endZoneSize);                         // Arduino plays on the left side (default: red)
}

//---
void prepareTrainingReturn() {
  if (trainingNeedsAutomaticReturn(selectedMode, ballDir)) {
    trainingHitPosition = chooseTrainingHitPosition();
  }
  else {
    trainingHitPosition = -1;
  }
}

//---
void performTrainingReturn() {
  dfpExecute(0x0F, 0x01, 2);                         // pong sound
  previousButtonPos = ballPos;
  previousButtonColor = playerColor[TRAINING_ARDUINO_PLAYER];
  previousButtonMillis = millis();
  ChangeDirection();
  previousMoveMicros = micros();
}

//---
void resetRallyeRound() {
  rallyeSuccessfulReturns = 0;
  endZoneSize = rallyeStartEndZoneSize;
}

//---
void updateRallyeAfterReturn() {
  if (selectedMode != MODE_RALLYE) {
    return;
  }

  rallyeSuccessfulReturns++;

  if (!rallyeShouldFlash(rallyeSuccessfulReturns,
                          RALLYE_RETURNS_PER_SHRINK,
                          endZoneSize,
                          rallyeMinimumEndZoneSize)) {
    return;
  }

  int oldZoneSize = endZoneSize;
  endZoneSize = rallyeZoneSize(rallyeStartEndZoneSize,
                               rallyeMinimumEndZoneSize,
                               rallyeShrinkStep,
                               RALLYE_RETURNS_PER_SHRINK,
                               rallyeSuccessfulReturns);

  if (endZoneSize < oldZoneSize) {
    flashRallyeShrink(oldZoneSize);
    previousMoveMicros = micros();
  }
}

//---
void flashRallyeShrink(int oldZoneSize) {
  for (byte flash = 0; flash < RALLYE_FLASH_COUNT; flash++) {
    GeneratePlayField(scoreDimBright);

    for (int i = endZoneSize; i < oldZoneSize; i++) {
      leds[i] = CHSV(0, 255, brightness);
      leds[NUM_LEDS - 1 - i] = CHSV(0, 255, brightness);
    }

    if (ballPos >= 0 && ballPos < NUM_LEDS) {
      leds[ballPos] = CHSV(0, 0, brightness);
    }

    FastLED.show();
    delay(RALLYE_FLASH_ON_MS);

    GeneratePlayField(scoreDimBright);
    if (ballPos >= 0 && ballPos < NUM_LEDS) {
      leds[ballPos] = CHSV(0, 0, brightness);
    }
    FastLED.show();
    delay(RALLYE_FLASH_OFF_MS);
  }
}

void CheckScore() {
  previousButtonPos = -1;                             // clear last ball-position at button-press

  if (ballPos < 0) {                                  // player1 made the point
    playerScore[1] += 1;                              // new score for player1

    GeneratePlayField(brightness);                    // show new score full bright
    BlinkNewScore(NUM_LEDS / 2 - 1 + playerScore[1], playerColor[1]); // blink last score

    playerStart = 1;                                  // define next player to start (player, who made the point)
  }
  else {                                              // player0 made the point
    playerScore[0] += 1;                              // new score for player0

    GeneratePlayField(brightness);    // show new score full bright
    BlinkNewScore(NUM_LEDS / 2 - playerScore[0], playerColor[0]); // blink last score

    playerStart = 0;                                  // define next player to start (player, who made the point)
  }

  GeneratePlayField(brightness);                      // show new score full bright
  FastLED.show();

  delay(1000);
}

void CheckWinner() {                                                    // check if we have a winner
  if (playerScore[0] >= winRounds || playerScore[1] >= winRounds)  {    // we have a winner!                         
    dfpExecute(0x0F,0x01,5);                                            // play winner sound

    activeGame = false;
    FastLED.clear();
    winRainbow(playerScore[0] > playerScore[1]);        // TRUE if player 0 won; FALSE when player 1 won
    memset(playerScore, 0, sizeof(playerScore));        // reset all scores
    playerStart = abs(playerStart - 1);                 // next game starts looser
  }
}

//---
void GeneratePlayField(byte bright) {
  FastLED.clear();                                      // clear all
  GenerateEndZone();                                    // generate endzone
  GenerateScore(bright);                                // generate actual score
  GenerateLastHit();                                    // generate mark of position of last button-press
}

//---
void GenerateEndZone() {
  for (int i = 0; i < endZoneSize; i++)
  {
    leds[i] = CHSV(endZoneColor, 255, brightness);
    leds[NUM_LEDS - 1 - i] = CHSV(endZoneColor, 255, brightness);
  }
}

//---
void GenerateScore(int bright) { 
  for (int i = 0; i < playerScore[0]; i++) {                                // Player 0
    leds[NUM_LEDS / 2 - 1 - i] = CHSV(playerColor[0], 255, bright);
  }

  for (int i = 0; i < playerScore[1]; i++) {                                // Player 1
    leds[NUM_LEDS / 2 + i] = CHSV(playerColor[1], 255, bright);
  }
}

//---
void GenerateLastHit() {
  if (previousButtonPos != -1 && previousButtonMillis + 500 > millis())
  {
    leds[previousButtonPos] = CHSV(previousButtonColor, 255, previousButtonBright);
  }
}

//---
void BlinkNewScore(int pos, byte color) {
  for (int i = 1; i <= 4; i++)
  {
    leds[pos] = CHSV(color, 255, (i % 2) * brightness);  // flash LED 2 times (1-0-1-0)
    FastLED.show();
    delay(300);
  }
}

//---
void winRainbow(boolean won) {
  for (int k = 0; k < 3; k++) {                 // 3 rounds rainbow
    for (int j = 0; j <= 255; j++) {
      for (int i = 0; i < NUM_LEDS / 2; i++) {
        if (won == true) {                      // player 0 won --> Rainbow left
          leds[i] = CHSV(((i * 256 / NUM_LEDS) + j) % 256, 255, brightness);
        }
        else {                                  // player 1 won --> Rainbow right
          leds[NUM_LEDS - i - 1] = CHSV(((i * 256 / NUM_LEDS) + j) % 256, 255, brightness);
        }
      }
      FastLED.show();
      delay(7);
    }
  }
}

//---
void Rainbow() {
  for (int j = 0; j <= 255; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV((((long)i * 256 / NUM_LEDS) + j) % 256, 255, brightness);
    }
    FastLED.show();
    delay(7);
  }
}

// --- Excecute the DFPlayer command with two parameters (folder and file numbers)
 void dfpExecute(byte CMD, byte Par1, byte Par2) {   
  # define Start_Byte     0x7E  
  # define Version_Byte   0xFF  
  # define Command_Length 0x06  
  # define Acknowledge    0x00   
  # define End_Byte       0xEF  
  // Calculate the checksum (2 bytes)  
  uint16_t checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);  
  // Build the command line  
  uint8_t Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,  
         Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};  
  // Send the command line to DFPlayer  
  for (byte i=0; i<10; i++) mySerial.write( Command_line[i]);  

  delay(50);
 }  


/*
  https://projecthub.arduino.cc/flyingangel/1d-pong-1a0acf
  by FlyingAngel - 18.4.2020
  Its a 1D Pong Game on an 1m, 60 Neopixel LED Strip
  1D-Pong

This is a 1D version of the popular 1972 released Pong from Atari. 2 players try to play a ball to the opponent. It's not played on a 2-dimensional screen, but on a 1-dimensional NeoPixel-Strip. So the rules are different, but not less fun!

I merged some ideas of different 1D-Pong games I found in internet and made my own version.
  https://www.vagrearg.org/content/onedpong
  https://www.instructables.com/id/Make-Your-Own-1D-Pong-Game/
  https://cdn.instructables.com/FMB/UTCY/JNSR62LE/FMBUTCYJNSR62LE.LARGE.jpg?auto=webp&width=1024&height=1024&fit=bounds
  https://www.youtube.com/watch?v=YpqpQz4mYZk

This adapted version is played on a 1 m NeoPixel strip with 144 LEDs; timing and zones are scaled from the original 60-LED design. Each player 
has a arcade-button with a LED in it. The LED shows, which player has to 
make the first move. The first player of a game is random.

When the ball is on its way, the other player tries to 
return the ball by pressing the button when the ball is in his "endzone", 
marked blue. You have only one try! When the hit is in the endzone, the ball 
returns and so on. Each time, the ball changes its direction, the ball speeds 
up. And you have the chance to speed up the ball by yourself: When you hit the 
button on the forelast field of your endzone, you activate a "Boost", on the 
last field a "Superboost".

Is the button pressed outside the endzone (too early or to late), it's a miss 
and the other player makes a score. Then the player who made the score starts 
the next round. 

In the middle of the strip you can see the actual score, which is dimmed during
the game.

The first player who wins 5 rounds, wins the game, and can enjoy a rainbow-
animation.


*/
