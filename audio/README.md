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
| 011-gruen.mp3 | Bedienhinweis | "Helligkeit mit grüner Taste einstellen" |
| 011-blau.mp3 | Bedienhinweis | "Helligkeit mit blauer Taste einstellen" |
| 012-gelb.mp3 | Bedienhinweis | "Spiel mit gelber Taste starten" |
| 012-blau.mp3 | Bedienhinweis | "Spiel mit blauer Taste starten" |
| 012-rot.mp3 | Bedienhinweis | "Spiel mit roter Taste starten" |

### Andere Tasten- und Spielerfarben verwenden

Die Farbe der Arcade-Taster und die Spielerfarben der LEDs sind technisch unabhängig voneinander. Werden andere Tasterfarben als Rot und Grün verwendet, müssen daher zwei Dinge angepasst werden:

1. Die gewünschten LED-Spielerfarben im Sketch über `playerColor[]` einstellen, siehe dazu auch [Firmware/README.md](/firmware/README.md).
2. Die passenden Sprachansagen aus `audio/alternative-Tastenfarben` auswählen und als `011.mp3` bzw. `012.mp3` auf die microSD-Karte kopieren.

Beispiel für einen **gelben linken Taster** und einen **blauen rechten Taster**:

```cpp
byte playerColor[] = {64, 128};    // Gelb & Hellblau
```

Dazu:

- `011-gelb.mp3` als `011.mp3` auf die Speicherkarte kopieren.
- `012-blau.mp3` als `012.mp3` auf die Speicherkarte kopieren.

Für den rechten Spieler wird im Beispiel bewusst der Farbwert `128` (Hellblau/Türkis) verwendet. Dadurch bleibt die Spielerfarbe deutlich von den blauen Rückschlagzonen mit dem Farbwert `160` unterscheidbar, obwohl physisch ein blauer Arcade-Taster verwendet wird.

