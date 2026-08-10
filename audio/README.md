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
| 011.mp3 | Bedienhinweis | "Helligkeit mit roter Taste einstellen" |
| 012.mp3 | Bedienhinweis | "Spiel mit grüner Taste starten" |
| 020.mp3 | Modus Normal | "Du hast den Anfänger-Modus ausgewählt" |
| 021.mp3 | Modus Pro | "Du hast den Profi-Modus ausgewählt" |
| 022.mp3 | Modus Insane | "Du hast den Insane-Modus ausgewählt" |
| 023.mp3 | Rallye-Modus | "Du hast den Rallye-Modus ausgewählt" |
| 024.mp3 | Training-Modus | "Du hast den Training-Modus ausgewählt" |

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
├── 012.mp3
├── 020.mp3
├── 021.mp3
├── 022.mp3
├── 023.mp3
└── 024.mp3
```

## Alternative Hinweisdateien für andere Tastenfarben

| Datei | Verwendung | gesprochener Text |
|---|---|---|
| 011-gelb.mp3 | Bedienhinweis | "Helligkeit mit gelber Taste einstellen" |
| 011-grün.mp3 | Bedienhinweis | "Helligkeit mit grüner Taste einstellen" |
| 011-blau.mp3 | Bedienhinweis | "Helligkeit mit blauer Taste einstellen" |
| 012-gelb.mp3 | Bedienhinweis | "Spiel mit gelber Taste starten" |
| 012-blau.mp3 | Bedienhinweis | "Spiel mit blauer Taste starten" |
| 012-rot.mp3 | Bedienhinweis | "Spiel mit roter Taste starten" |

## Verwendung der alternativen Farben
Um die alternativen Dateien zu verwenden, müssen die jeweils ausgewählten Kombinationen jeweils als Datei 011.mp3
und 012.mp3 auf die Speicherkarte kopiert werden.
Zusätzlich sollten im Programmcode die Farben der Spieler angepasst werden, für Details siehe [Firmware/README.md](firmware/README.md).
