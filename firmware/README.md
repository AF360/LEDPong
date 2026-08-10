# Firmware

Für den LEDPong-Sketch (LEDPong_v1.1.0.ino) müssen zusätzliche folgende Arduino-Bibliotheken in der Arduino-IDE installiert werden:

- FastLED by Daniel Garcia (verwendet: Version 3.10.5)
- DFRobotDFPlayerMini by DFRobot (verwendet: Version 1.0.6)

## Spielerfarben anpassen

Standardmäßig verwendet LEDPong **Rot** für Spieler 1 und **Grün** für Spieler 2. Die Farben werden im Sketch über folgende Zeile festgelegt:

```cpp
byte playerColor[] = {0, 96};    // Rot & Grün
```

Die Werte entsprechen dem Farbton (`Hue`) des von FastLED verwendeten HSV-Farbmodells. Einige geeignete Beispiele:

| Farbe | Wert |
|---|---:|
| Rot | `0` |
| Gelb | `64` |
| Grün | `96` |
| Hellblau / Türkis | `128` |
| Blau | `160` |
| Violett | `192` |

Beispiel für **Gelb und Hellblau**:

```cpp
byte playerColor[] = {64, 128};
```

Die Rückschlagzonen verwenden standardmäßig den Wert `160` (Blau). Dieser Farbwert sollte daher möglichst **nicht als Spielerfarbe** verwendet werden, damit Spieler- und Rückschlagzonen eindeutig unterscheidbar bleiben.

Werden andere Tastenfarben verwendet, können zusätzlich die entsprechenden alternativen Sprachansagen aus dem Verzeichnis `audio/Alternative-Tastenfarben` verwendet werden.
