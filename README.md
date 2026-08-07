![LEDPong](images/ledpong.png)

# LEDPong

LEDPong ist eine moderne Interpretation des klassischen Pong-Spiels.
Gespielt wird nicht auf einem Bildschirm, sondern auf einem **1 m langen LED-Strip mit 144 adressierbaren WS2812B-LEDs**.

Zwei Spieler versuchen, den als leuchtende LED dargestellten Ball mit ihren Arcade-Tastern im richtigen Moment zurückzuspielen. Mehrere Schwierigkeitsstufen, Sprachansagen, Soundeffekte, eine Lautstärkeregelung sowie ein Demo- und ein Scanner-Modus ergänzen das Spiel.

## Spielmodi

1. **Anfänger-Modus** (große Rückschlagzonen, niedrige Geschwindigkeit und Beschleunigung)
2. **Profi-Modus** (kleinere Rückschlagzonen, höhere Geschwindigkeit und Beschleunigung)
3. **Insane-Modus** (stark verkleinerte Rückschlagzonen und höhere Beschleunigug)
4. **Rallye-Modus** (nach jeweils drei erfolgreichen Rückschlägen werden beide Rückschlagzonen verkleinert)
5. **Training-Modus** (ein Spieler tritt auf der grünen Seite gegen den Arduino an)

## Besonderheiten

- 🎮 Zwei Arcade-Taster für die Spieler
- 🔊 Sprachansagen und Soundeffekte über DFPlayer Mini
- 🌈 144 adressierbare WS2812B-LEDs
- 🎚 Lautstärkeregelung per Potentiometer
- 🎛 5-fach-Moduswahlschalter
- 🚨 Scanner-Modus mit Licht- und Soundeffekt
- 🖨 Druckfertige 3D-Modelle für Gehäuse- und Bedienelemente
- 🔧 Vollständige Aufbau-, Verdrahtungs- und Bedienungsdokumentation

## Dokumentation und Dateien

- **[Dokumentation](docs/README.md)** – Stückliste, Aufbauanleitung, Verdrahtungsplan und Bedienungsanleitung
- **[Firmware](firmware/README.md)** – Arduino-Sketch und benötigte Bibliotheken
- **[Audiodateien](audio/README.md)** – Sprachansagen und Soundeffekte
- **[Gehäuse und 3D-Druck](gehäuse/README.md)** – Hinweise zum Kabelkanal und zu den STL-Dateien
- **[Release V1.0.0](https://github.com/AF360/LEDPong/releases/tag/V1.0.0)** – erster stabiler und vollständig dokumentierter Veröffentlichungsstand

## Lizenz und Danksagungen

LEDPong wird unter der **GNU General Public License v3.0 oder später** veröffentlicht.
Weitere Informationen zu den ursprünglichen Projekten und Mitwirkenden befinden sich in **[AUTHORS.md](AUTHORS.md)**.

![LEDPong-Logo](images/ledponglogo.png)
