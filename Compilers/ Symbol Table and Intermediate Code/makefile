GCC = g++
ROB = robin
LEX = robin
all: 1

1: fl.o bi.o $(ROB).tab.o lex.yy.o Node.o SymbolTable.o Object.o quad.o
	$(GCC) -o 1 $(ROB).tab.o lex.yy.o Node.o SymbolTable.o Object.o quad.o -lfl

fl.o: $(LEX).l
	flex $(LEX).l

bi.o: $(ROB).y
	bison -d -v $(ROB).y

lex.yy.o: lex.yy.c
	$(GCC) -c lex.yy.c

$(ROB).tab.o: $(ROB).tab.c
	$(GCC) -c $(ROB).tab.c

Node.o : Node.h Node.cpp
	$(GCC) -c Node.cpp

SymbolTable.o : SymbolTable.h SymbolTable.cpp
	$(GCC) -c SymbolTable.cpp
Object.o : Object.h Object.cpp
	$(GCC) -c Object.cpp


quad.o : quad.h quad.cpp
	$(GCC) -c quad.cpp
