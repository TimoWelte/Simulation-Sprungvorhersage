# Simulation-Sprungvorhersage
Simulation welche verschiedene Branch-Prädiktoren simuliert

*Programm ausführen:*

Das Programm wurde mit Visual Studio erstellt, sofern Visual Studio installiert ist, kann das Projekt (die Solution) auch damit geöffnet und ausgeführt werden. 
Andernfalls finden sich sowohl die C++ code Dateien (.h & .cpp) als auch das Programm als „.exe“ im Ordner „Predictor Simulation“.

*Programmablauf:*

Zuerst werden die Trace files (müssen sich im Ordner “Trace files” befinden) in das Programm eingelesen. Dazu wird für jede Trace Datei ein großer Vektor mit einem Paar aus zwei Integern erstellt (vector<pair<address_t , int>>). Hierbei stellt das erste Element des Paars die Adresse des Branches (umgewandelt in einen 8-Byte Integer) dar, während der zweite Integer entweder 0 (kein Sprung) oder 1 (Sprung) sein kann. 
Anschließend wird der Benutzer gefragt, auf welchem Branch Prädiktor die Simulation ausgeführt werden soll. Zu jedem Prädiktor gibt es Parameter, die ebenfalls vom Benutzer angegeben werden müssen. Falls alle Parameter korrekt angegeben wurden, wird für jeden Test die Simulation ausgeführt und die Präzision anschließend ausgegeben. Zusätzlich wird jeder Simulationsdurchlauf in einer Datei abgespeichert. (Für jeden Prädiktor wird hier eine eigene “Result” Datei erzeugt)
Um die Simulation durchführen zu können steht für jeden Prädiktor eine eigene Klasse zur Verfügung, mit der der Prädiktor (abhängig von den angegebenen Parametern) erzeugt wird. 
Außerdem enthält jede Klasse (jeder Prädiktor) eine Methode “RunTestSet”, welche jeweils eine Trace Datei auswertet (die Trace Datei muss im obigen Format angegeben werden) und die Präzision zurückgibt.
Nachdem ein Prädiktor simuliert wurde, kann ein neuer Prädiktor ausgewählt werden und eine neue Simulation gestartet werden. Die Anwendung läuft so lange, bis die Konsole manuell geschlossen wird. 
