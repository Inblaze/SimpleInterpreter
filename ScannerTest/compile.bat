g++ -c scanner.cpp
g++ -c scannerMain.cpp
g++ -o ScannerTest.exe scanner.o scannerMain.o
del *.o /f /q