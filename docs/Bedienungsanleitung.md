# LEDPong Bedienungsanleitung

## Einschalten und Moduswahl

Nach dem Einschalten beziehungsweise dem Anlegen der 5V-Versorgungsspannung startet LEDPong mit einem Regenbogeneffekt auf dem LED-Streifen. Anschließend folgen eine Willkommensansage und eine kurze Erläuterung zur Einstellung der LED-Helligkeit.

Mit dem 5-fach-Moduswahlschalter wird der gewünschte Spielmodus ausgewählt:

1. Anfänger-Modus
2. Profi-Modus
3. Insane-Modus
4. Demo-Modus
5. Scanner-Modus – Larson-Scanner als kleiner Gag

Die Helligkeit der LEDs lässt sich durch mehrfaches Drücken der roten Taste in fünf Stufen durchschalten.

Ein Druck auf die grüne Taste startet den ausgewählten Modus.

## Spielstart

Beim ersten Match nach dem Einschalten wird der beginnende Spieler zufällig bestimmt. Am entsprechenden Ende des LED-Streifens leuchtet die Start-LED in der Farbe des Spielers.

Dieser Spieler muss seinen eigenen Taster drücken, um den Ball abzuschießen.

Nach einem Punkt eröffnet der Spieler die nächste Runde, der den Punkt erzielt hat. Nach einem vollständig beendeten Match beginnt im nächsten Match der vorherige Verlierer.

## Spielablauf

Der Ball bewegt sich als leuchtende LED über den Streifen.

Die blauen Bereiche an beiden Enden markieren die Rückschlagzone des jeweiligen Spielers. Je höher der eingestellte Schwierigkeitsgrad ist, desto kleiner wird diese Zone.

Nur innerhalb der blauen Rückschlagzone kann der Ball durch rechtzeitiges Drücken des eigenen Tasters erfolgreich zurückgespielt werden. Je näher der Ball am Ende des Spielfeldes getroffen wird, desto höher ist seine Rückspielgeschwindigkeit. Auch der gewählte Schwierigkeitsgrad beeinflusst die Ballgeschwindigkeit.

Mit jedem erfolgreichen Rückschlag wird der Ball zusätzlich etwas schneller. Dadurch steigt der Schwierigkeitsgrad im Verlauf einer Runde kontinuierlich an.

Erreicht der Ball die gegenüberliegende Rückschlagzone, muss der jeweilige Spieler seinen Taster im richtigen Moment betätigen. Erfolgt der Tastendruck innerhalb des erlaubten Bereichs, wird der Ball zurückgeschlagen und bewegt sich in die entgegengesetzte Richtung.

Wird zu früh oder zu spät gedrückt, geht der Ball verloren. Der Gegenspieler erhält einen Punkt, und in der Mitte des LED-Streifens wird der aktuelle Spielstand angezeigt. Anschließend beginnt die nächste Runde.

Das Match endet, sobald ein Spieler fünf Punkte erreicht hat.

## Lautstärke und Ton

Die Lautstärke wird über das Potentiometer eingestellt. Mit dem Ein-/Ausschalter für den Lautsprecher kann die Tonausgabe vollständig stummgeschaltet werden.

## Demo-Modus

Im Demo-Modus übernimmt der Arduino beide Spieler und spielt selbstständig. Die Rückschlagpositionen innerhalb der Endzonen werden zufällig gewählt.

Gelegentlich wird ein Ball absichtlich nicht zurückgeschlagen, damit auch die Punkteanzeige und die Siegeranimation vorgeführt werden. Nach dem Ende eines Matches beginnt automatisch eine neue Demonstration.

## Scanner-Modus

Im Scanner-Modus läuft ein rotes Licht mit Leuchtspur fortlaufend über den LED-Streifen hin und her. Gleichzeitig wird der passende Scanner-Sound abgespielt.

Dieser Modus dient ausschließlich als Showeffekt und enthält kein Pong-Spiel.

Demo- und Scanner-Modus können durch Drehen des Moduswahlschalters oder durch Drücken einer der beiden Spielertasten beendet werden.
