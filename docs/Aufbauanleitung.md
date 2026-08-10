# LEDPong Aufbauanleitung

## 1. Komponenten vorbereiten

Alle benötigten Komponenten und Werkzeuge gemäß Stückliste bereitlegen und die benötigten 3D-Druckteile ausdrucken.

## 2. Arduino Nano vorbereiten

Die Arduino IDE installieren und starten. Anschließend den Sketch `LEDPong_v1.1.0.ino` aus dem Verzeichnis `firmware` dieses Repositorys öffnen.

Den Arduino Nano per USB mit dem Computer verbinden.

In der Arduino IDE folgende Einstellungen vornehmen:

- **Tools → Board → Arduino AVR Boards → Arduino Nano**
- **Tools → Port → passenden Port auswählen**

Der richtige Port ist normalerweise derjenige, der nach dem Anschließen des Arduino Nano neu in der Liste erscheint.

Über das Bibliotheks-Symbol am linken Fensterrand die Bibliotheksverwaltung öffnen und folgende Bibliotheken installieren:

- **FastLED** von Daniel Garcia  
  Getestete Version: 3.10.5
- **DFRobotDFPlayerMini** von DFRobot  
  Getestete Version: 1.0.6

Danach den Sketch über **Sketch → Verify/Compile** überprüfen und kompilieren. Wenn keine Fehler auftreten, den Sketch über **Sketch → Upload** auf den Arduino Nano übertragen.

Der Arduino Nano ist damit für den Einbau vorbereitet.

> **Tipp:** Falls der Upload bei einem Nano-Klon fehlschlägt, unter **Tools → Processor** testweise **ATmega328P (Old Bootloader)** auswählen.

## 3. MicroSDHC-Karte vorbereiten

Die MicroSDHC-Karte über einen Kartenleser mit dem Computer verbinden und mit dem Dateisystem **FAT32** formatieren.

Im Hauptverzeichnis der Karte einen Ordner mit dem Namen `01` anlegen. Anschließend die Audiodateien `001.mp3` bis `024.mp3` in diesen Ordner kopieren.

Die Karte danach sicher auswerfen und in den DFPlayer Mini einsetzen.

> **Tipp:** Für eine möglichst hohe Kompatibilität sollte eine kleine MicroSDHC-Karte verwendet werden. Die in der Stückliste empfohlene 4-GB-Karte funktioniert zuverlässig. Größere Karten können je nach Hersteller und Formatierung Probleme verursachen.

## 4. Kabelkanal zuschneiden

Den Kabelkanal auf eine Länge von etwa **1.100 bis 1.150 mm** kürzen.

Die Schnittkanten anschließend entgraten.

## 5. Bohrungen im Deckel markieren

Den LED-Strip mittig auf den Deckel des Kabelkanals legen, zunächst jedoch noch nicht festkleben.

Die Positionen der Anschlussleitungen an beiden Enden des LED-Strips auf dem Deckel markieren.

An beiden Enden des Deckels außerdem die Positionen der beiden Arcade-Taster anzeichnen.

Vor dem Bohren prüfen, ob unterhalb der markierten Positionen ausreichend Platz für Taster, Leitungen und Befestigungsteile vorhanden ist.

## 6. Seitliche Bohrungen markieren und herstellen

Die benötigten Bohrdurchmesser der verwendeten Schalter und Bedienelemente mit einer Schieblehre ermitteln.

Anschließend die Positionen für folgende Komponenten an der Seite des Kabelkanals festlegen und markieren:

- Moduswahlschalter
- USB-C-Spannungszuführung
- Lautstärke-Potentiometer
- Lautsprecher-Ein-/Ausschalter

Die Bohrungen mit den jeweils passenden Durchmessern herstellen und entgraten.

Etwa in der Mitte des Kabelkanals für den Lautsprecher neun kleine Bohrungen mit ungefähr **3 mm Durchmesser** stern- oder rautenförmig anordnen.

Der Lautsprecher wird später von innen hinter diese Öffnungen geklebt, damit der Schall möglichst ungehindert nach außen gelangen kann.

## 7. Verkabelung vorbereiten

Die USB-C-Panelbuchse in die Endkappe einsetzen und verschrauben beziehungsweise einkleben.

Die Länge des Anschlusskabels der USB-C-Panelbuchse bestimmt die spätere Position des Arduino Nano innerhalb des Kabelkanals. Die Panelbuchsen sind beispielsweise mit 0,3 m, 0,5 m oder 0,9 m Kabellänge erhältlich.

Von der vorgesehenen Arduino-Position aus die benötigten Leitungslängen zu folgenden Komponenten ermitteln:

- Arcade-Taster
- Moduswahlschalter
- Lautstärke-Potentiometer
- Lautsprecher-Schalter
- LED-Strip
- DFPlayer Mini

Die Litzen passend ablängen und an die jeweiligen Schalter und Bedienelemente anlöten.

Anschließend die Komponenten in das Gehäuse einklipsen oder mit ihren Überwurfmuttern befestigen.

> **Tipp:** Für WAGO-221-Klemmen die Litzen etwa **11 mm** abisolieren. Für direkte Lötverbindungen reichen normalerweise **4 bis 5 mm**.

> **Tipp:** Potentiometer und Moduswahlschalter besitzen häufig eine kleine Verdrehsicherungsnase. Dafür kann von innen eine passende kleine Bohrung oder Vertiefung mit einem Bohrer beziehungsweise Fräser hergestellt werden.

## 8. LED-Strip montieren

Den LED-Strip mittig auf den Deckel des Kabelkanals kleben und seine Anschlussleitungen durch die zuvor gebohrten Öffnungen führen.

Die Enden des LED-Strips anschließend mit den gedruckten Abdeckkappen verdecken.

Die Abdeckkappen können verklebt oder verschraubt werden. Bei einer Verschraubung können die Schrauben von innen durch den Deckel in die Kappen geführt werden, damit außen keine Schraubenköpfe sichtbar sind.

> **Wichtig:** Darauf achten, dass sich der mit **DIN** gekennzeichnete Dateneingang des LED-Strips in der Nähe des Arduino Nano befindet. Der Arduino darf nicht versehentlich mit der Datenausgangsseite des Strips verbunden werden, sonst bleibt der Strip dunkel.

## 9. Spannungsversorgung verkabeln

Während der gesamten Verkabelung darf keine Spannungsquelle angeschlossen sein.

Die 5V- und GND-Leitungen der USB-C-Spannungszuführung mit den beiden vorgesehenen Kondensatoren verbinden. Beide Kondensatoren werden parallel zwischen **+5V** und **GND** angeschlossen.

Bei einem Elektrolytkondensator unbedingt auf die richtige Polarität achten:

- Pluspol an +5V
- Minuspol an GND

Anschließend eine sternförmige 5V-Verteilung und eine gemeinsame GND-Verteilung aufbauen. Dafür eignen sich beispielsweise WAGO-221-Klemmen.

An die gemeinsame 5V-Verteilung werden angeschlossen:

1. Arduino Nano, Pin `5V`
2. DFPlayer Mini, Pin `VCC`
3. LED-Strip, Anschluss `5V`
4. Lautstärke-Potentiometer, äußerer Anschluss

An die gemeinsame GND-Verteilung werden angeschlossen:

1. Arduino Nano, Pin `GND`
2. DFPlayer Mini, Pin `GND`
3. LED-Strip, Anschluss `GND`
4. Lautstärke-Potentiometer, äußerer Anschluss
5. Mittelkontakt des Moduswahlschalters
6. Massekontakt des roten Arcade-Tasters
7. Massekontakt des grünen Arcade-Tasters

Falls das verwendete DFPlayer-Mini-Modul zwei GND-Pins besitzt, sollten beide mit der gemeinsamen Masse verbunden werden.

Der mittlere Schleiferanschluss des Lautstärke-Potentiometers wird gemäß Anschlussplan mit dem Analogeingang `A1` des Arduino verbunden.

> **Tipp:** Wird die Lautstärke beim Drehen im Uhrzeigersinn leiser statt lauter, müssen die beiden äußeren Anschlüsse für 5V und GND am Potentiometer vertauscht werden.

> **Tipp:** Niemals mehrere Litzen gemeinsam in eine einzelne Federklemme stecken. Werden mehr Anschlüsse benötigt, eine weitere 3-fach- oder 5-fach-WAGO-221-Klemme über eine kurze Litzenbrücke anschließen. Für diese Brücke eignet sich beispielsweise AWG18- oder AWG20-Litze.

## 10. Lautsprecher anschließen

Den Lautsprecher direkt mit den Anschlüssen `SPK+` und `SPK-` des DFPlayer Mini verbinden.

Der Lautsprecher-Ein-/Ausschalter wird in Reihe in eine der beiden Lautsprecherleitungen eingeschleift.

Den Lautsprecher anschließend von innen hinter die stern- oder rautenförmig angeordneten 3-mm-Bohrungen kleben.

Dabei darauf achten, dass die Lautsprechermembran nicht mit Klebstoff in Berührung kommt und frei schwingen kann.

## 11. Signalverkabelung herstellen

Alle verbleibenden Leitungen gemäß Anschluss- und Pinbelegungsplan mit den korrekten Anschlüssen verbinden beziehungsweise verlöten.

Besonders auf folgende Verbindungen achten:

- Datenleitung des LED-Strips an den vorgesehenen Arduino-Pin
- RX- und TX-Verbindungen des DFPlayer Mini
- Signalleitungen der beiden Arcade-Taster
- Schleifer des Lautstärke-Potentiometers
- Kontakte des 5-fach-Moduswahlschalters

Nach Abschluss der Lötarbeiten alle Lötstellen auf Kurzschlüsse, lose Litzen und versehentliche Verbindungen zu benachbarten Kontakten prüfen.

## 12. Funktionstest durchführen

Vor dem ersten Einschalten mit einem Multimeter prüfen, ob zwischen **+5V** und **GND** kein Kurzschluss besteht.

Danach über ein geeignetes 5V-Netzteil oder eine ausreichend leistungsfähige Powerbank Spannung zuführen.

Folgende Funktionen testen:

- Startsequenz und Sprachausgabe
- LED-Strip
- roter und grüner Arcade-Taster
- Moduswahlschalter
- Lautstärke-Potentiometer
- Lautsprecher-Ein-/Ausschalter
- alle Spielmodi

Treten Probleme auf, die Spannungsversorgung sofort trennen und die Verkabelung erneut prüfen.

## 13. Gehäuse schließen

Die Endkappe mit der USB-C-Panelbuchse in den Kabelkanal einsetzen.

Das interne USB-Kabel mit dem USB-Anschluss des Arduino Nano verbinden und so verlegen, dass es nicht unter Spannung steht oder beim Schließen des Gehäuses eingeklemmt wird.

Alle Leitungen geordnet im Kabelkanal verstauen. Darauf achten, dass keine Litzen in den Bereich der Deckelrastung gelangen.

Anschließend den Deckel auf den Kabelkanal setzen und die zweite Endkappe montieren.

## 14. Fertig

LEDPong ist nun einsatzbereit.

Viel Spaß beim Spielen!

## Das fertige LEDPong:

![LEDPong1](/images/ledpong-gesamtansicht.jpg)
Gesamtansicht mit LED-Rainbow während des Startvorgangs

![LEDPong2](/images/ledpong-spieler-endzone.jpg)
Rückschlagzone, Abdeckkappe, 5-fach Moduswahlschalter, USB-C für Spannungsversorgung, hier mit Powerbank im Einsatz

![LEDPong3](/images/ledpong-bedienelemente.jpg)
Score-Anzeige, Lautstärkeregler, Lautsprecher Ein-/Ausschalter

![LEDPong4](/images/ledpong-usb-c-endkappe.jpg)
Endkappe m. USB-C Serviceport zum flashen des Arduino, 30mm Arcade Button, LED-Strip-Abdeckkappe
