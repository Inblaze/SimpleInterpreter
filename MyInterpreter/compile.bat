g++ -c scanner.cpp
g++ -c parser.cpp
g++ -c my_semantics.cpp
g++ -c main.cpp
g++ -o MyInterpreter.exe scanner.o parser.o my_semantics.o main.o
del *o /f /q