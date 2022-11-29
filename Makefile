output: main.o SymbolTable.o ErrorCheck.o Expressions.o LiteralTable.o Instructions.o FileHandler.o PassTwo.o
	g++ -o output main.o SymbolTable.o ErrorCheck.o Expressions.o LiteralTable.o Instructions.o FileHandler.o PassTwo.o -std=c++11

main.o: main.cpp
	g++ -c main.cpp -std=c++11

SymbolTable.o: SymbolTable.h SymbolTable.cpp
	g++ -c SymbolTable.cpp -std=c++11

ErrorCheck.o: ErrorCheck.h ErrorCheck.cpp
	g++ -c ErrorCheck.cpp -std=c++11

Expressions.o: Expressions.h Expressions.cpp
	g++ -c Expressions.cpp -std=c++11

LiteralTable.o: LiteralTable.h LiteralTable.cpp
	g++ -c LiteralTable.cpp -std=c++11

Instructions.o: Instructions.h Instructions.cpp
	g++ -c Instructions.cpp -std=c++11

FileHandler.o: FileHandler.h FileHandler.cpp
	g++ -c FileHandler.cpp -std=c++11

PassTwo.o: PassTwo.h PassTwo.cpp
	g++ -c PassTwo.cpp -std=c++11

clean:
	rm output main.o SymbolTable.o ErrorCheck.o Expressions.o LiteralTable.o Instructions.o FileHandler.o PassTwo.o sic2.int