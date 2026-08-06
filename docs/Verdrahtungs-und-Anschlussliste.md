# LEDPong Verdrahtung 

## Arduino Nano

| Nano-Pin | Ziel | Elektrische Funktion |
|---|---|---|
| 5V | gemeinsamer geregelter +5V-Bus | Versorgung |
| GND | gemeinsamer GND-Bus | Masse |
| D2 | Taster Spieler 0 → GND | `INPUT_PULLUP`, aktiv LOW |
| D3 | Taster Spieler 1 → GND | `INPUT_PULLUP`, aktiv LOW |
| D4 | DFPlayer TX | SoftwareSerial RX des Nano |
| D5 | über 1 kΩ zu DFPlayer RX | SoftwareSerial TX des Nano |
| D6 | WS2812B DIN | LED-Datensignal |
| D7 | Drehschalter Position 1 → GND über COM | Modus Normal, aktiv LOW |
| D8 | Drehschalter Position 2 → GND über COM | Modus Pro, aktiv LOW |
| D9 | Drehschalter Position 3 → GND über COM | Modus Insane, aktiv LOW |
| D10 | Drehschalter Position 4 → GND über COM | Modus Demo, aktiv LOW |
| D11 | Drehschalter Position 5 → GND über COM | Modus Larson-Scanner, aktiv LOW |
| A0 | unbeschaltet | Rauschquelle für `randomSeed(analogRead(0))` |
| A1 | Schleifer des 10-kΩ-Potis | Lautstärke; GND = Minimum, +5V = Maximum |

## DFPlayer Mini

| DFPlayer-Pin | Ziel |
|---|---|
| VCC | +5V |
| GND | gemeinsamer GND |
| TX | Nano D4 |
| RX | Nano D5 über 1 kΩ Serienwiderstand |
| SPK+ | über SPST-Ein/Aus-Schalter zum Lautsprecher + |
| SPK− | direkt zum Lautsprecher − |

Der Lautsprecherausgang ist gebrückt. Weder SPK+ noch SPK− darf mit GND verbunden werden.

## WS2812B-Strip

- 1 Meter, 144 LEDs (`NUM_LEDS = 144`)
- +5V an geregelte 5V-Versorgung
- GND an gemeinsame Masse
- DIN an Nano D6 (Achtung: Die WS2812B Strips haben eine Datenrichtung! Der Anschluss des Arduino muss am Eingang des Strips erfolgen.)
- Stromversorgung passend zur tatsächlichen Helligkeit dimensionieren

## Bedienelemente

- Spieler-0-Taster: Nano D2 ↔ Taster ↔ GND
- Spieler-1-Taster: Nano D3 ↔ Taster ↔ GND
- 5-fach-Drehschalter: Positionen 1–5 an D7–D11, gemeinsamer Anschluss COM an GND
- Lautstärke-Poti 10 kΩ: Endanschlüsse an GND und +5V, Schleifer an A1
- Speaker-Mute: SPST-Schalter in Reihe zwischen DFPlayer SPK+ und Lautsprecher+

## Versorgung

- Eine gemeinsame geregelte +5V-Quelle
- Gemeinsame Masse für Nano, DFPlayer, LED-Strip, Taster, Drehschalter und Poti
- 100 nF und 100 µF parallel zwischen +5V und GND nahe der Steuerelektronik
- Externe +5V an den 5V-Pin des Nano, nicht an VIN
- Keine zweite USB-Stromquelle gleichzeitig anschließen
