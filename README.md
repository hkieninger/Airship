# Abstract: Blimp
Ziel unseres Projektes war ein ferngesteuertes Prallluftschiff zu bauen.
Wir experimentierten mit unterschiedlichen Materialien und Verfahren, um eine möglichst gasdichte Hülle zu fabrizieren. Die Hülle dient als Grundgerüst, an dem die Ruder und Gondel samt Motoren und Sensoren befestigt werden. Durch einen leichten Überdruck erhält die Hülle Stabilität und ihre Form, welche durch einen Kompromiss zwischen einem kleinem Oberflächen-zu-Volumen-Verhältnis und Aerodynamik entstand. Des Weiteren überlegten wir uns verschiedene Steuerungsmechanismen mit deren Vor- und Nachteilen.

Eine grosse Herausforderung bestand darin, das Gewicht des Luftschiffes zu optimieren. Denn ein geringeres Gewicht erlaubt das Traggasvolumen zu reduzieren, was wiederum zu einer kleineren und leichteren Hülle führt. Als Traggas entschieden wir uns Helium, statt Wasserstoff zu verwenden, damit wir auch innerhalb von Gebäuden fliegen dürfen. Mit «Lillo’s Fitness-Träff» fanden wir einen Sponsor für das kostspielige Helium.

Als zentrale Steuerungseinheit am Luftschiff fungiert ein Kleinstcomputer (Raspberry Pi). Er empfängt über WLAN die Steuersignale, verarbeitet diese und betätigt die Motoren und Servos zur Steuerung des Luftschiffs. Gleichzeitig liest er die in der Gondel befestigten Kameras und Sensoren aus. Mit den Sensoren kann die Position und Lage erfasst, Hindernisse erkannt und der Systemstatus geprüft werden.

Die zugehörige Software lässt sich in zwei Teile aufspalten: zum einen die Benutzeroberfläche, die auf dem Rechner des Piloten läuft und zum anderen die Kontrollsoftware auf dem zuvor genannten Kleinstcomputer. Für die Benutzeroberfläche musste das Verwalten der Verbindung, der eingehenden Daten und das effiziente Dekodieren der Videostreams gelöst werden. Für die Kontrollsoftware standen die Bibliotheken für die Sensoren und die Ausnahmebehandlung für die  Luftschiffsteuerung im Vordergrund.

Das Luftschiff wiegt samt Hülle leicht über 450 g, ist rund 2.40 m lang und hat einen Durchmesser von 60 cm. Dank dem geringen Gewichts dürfen wir rechtlich über Menschenansammlungen fliegen. Zudem kann das Luftschiff im Vergleich zu einer Drohne länger in der Luft bleiben.

![Image](image.jpg)
