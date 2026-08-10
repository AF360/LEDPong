# LEDPong Bedienungsanleitung

## Einschalten und Moduswahl

Nach dem Einschalten beziehungsweise dem Anlegen der 5V-Versorgungsspannung startet LEDPong mit einem Regenbogeneffekt auf dem LED-Streifen. Anschließend folgen eine Willkommensansage und eine kurze Erläuterung zur Einstellung der LED-Helligkeit.

Mit dem 5-fach-Moduswahlschalter wird der gewünschte Spielmodus ausgewählt:

1. Anfänger-Modus
2. Profi-Modus
3. Insane-Modus
4. Rallye-Modus
5. Training-Modus 

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

## Rallye-Modus

Der Rallye-Modus beginnt mit den Spieleinstellungen und der Rückschlagzonengröße des Profi-Modus. Lediglich die maximal mögliche Ballgeschwindigkeit entspricht dem Insane-Modus. Nach jeweils drei erfolgreichen Rückschlägen werden beide Rückschlagzonen gleichzeitig kleiner. Die wegfallenden LEDs blinken zweimal rot, bevor das Spiel mit den kleineren Zonen weiterläuft. Nach jedem Punkt werden die Rückschlagzonen für die nächste Rallye auf ihre volle Größe zurückgesetzt.

## Training-Modus

Im Training-Modus spielt eine Person auf der grünen Seite gegen den Arduino auf der roten Seite. Die rote Taste behält vor dem Spiel ihre normale Funktion zur Helligkeitseinstellung; die grüne Taste startet das Training und dient anschließend als Spielertaste. Während des Matches wird die rote Spielertaste ignoriert.

Der Training-Modus verwendet die vollständigen Spieleinstellungen des Profi-Modus. Beim Aufrufen des Training-Modus wird der erste Aufschlag zufällig bestimmt. Beginnt Grün, wartet LEDPong auf den Aufschlag mit der grünen Taste. Beginnt Rot, schlägt der Arduino nach einer kurzen sichtbaren Pause automatisch auf. Nach einem Punkt schlägt wie in den anderen Spielmodi der Spieler auf, der den Punkt erzielt hat. Nach einem abgeschlossenen Match beginnt beim nächsten Start der vorherige Verlierer.

Der Arduino wählt seine Rückschlagposition innerhalb der roten Rückschlagzone zufällig. Durchschnittlich ungefähr jeden zehnten Rückschlag verfehlt er absichtlich, damit der menschliche Spieler Punkte erzielen kann. Das Match endet regulär bei fünf Punkten und verwendet die vorhandene Punkte- und Siegeranzeige.

## Lautstärke und Ton

Die Lautstärke wird über das Potentiometer eingestellt. Mit dem Ein-/Ausschalter für den Lautsprecher kann die Tonausgabe vollständig stummgeschaltet werden.

![LEDPong](/images/ledpong-spielerperspektive.jpg)
