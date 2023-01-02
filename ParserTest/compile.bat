g++ -c scanner.cpp
g++ -c parser.cpp
g++ -c parserMain.cpp
g++ -o ParserTest.exe scanner.o parser.o parserMain.o
del *.o /f /q