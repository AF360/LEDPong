# Audiodateien

Die mit LEDPong ausgelieferten Sprachansagen und Soundeffekte wurden speziell für dieses Projekt erstellt.
Die Audiodateien können bei Bedarf auch durch eigene Dateien ersetzt werden. Damit die Firmware weiterhin funktioniert, müssen die Dateinamen und die Ordnerstruktur dabei unverändert bleiben.

## Aktuelle Belegung

| Datei | Verwendung | gesprochener Text |
|---|---|---|
| 001.mp3 | Spiel beginnt | |
| 002.mp3 | Ball getroffen | |
| 003.mp3 | Ball verfehlt | |
| 004.mp3 | Aufschlag | |
| 005.mp3 | Spiel gewonnen | |
| 010.mp3 | Begrüßung | "Hallo. Willkommen zu Pong" |
| 011.mp3 | Bedienhinweis | "Helligkeit mit roter Taste einstellen. Spiel mit grüner Taste starten" |
| 020.mp3 | Modus Normal | "Du hast den Anfänger-Modus ausgewählt" |
| 021.mp3 | Modus Pro | "Du hast den Profi-Modus ausgewählt" |
| 022.mp3 | Modus Insane | "Du hast den Insane-Modus ausgewählt" |
| 023.mp3 | Demo-Modus | "Du hast den Demo-Modus ausgewählt" |
| 024.mp3 | Scanner-Modus | "Du hast den Scanner-Modus ausgewählt" |
| 025.mp3 | Scanner-Geräusch als Loop | |

## Ordnerstruktur auf der microSD-Karte
Die Firmware erwartet dreistellig nummerierte Dateien im Ordner `01` der microSD-Karte des DFPlayerMini.

```text
01/
├── 001.mp3
├── 002.mp3
├── 003.mp3
├── 004.mp3
├── 005.mp3
├── 010.mp3
├── 011.mp3
├── 020.mp3
├── 021.mp3
├── 022.mp3
├── 023.mp3
├── 024.mp3
└── 025.mp3
