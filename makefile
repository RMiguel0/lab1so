CC = g++

all: program

program: LAB1_Delgado_Rivero.cpp
	$(CC) -std=c++17 -o program LAB1_Delgado_Rivero.cpp

run: program
	./program

clean:
	rm -f program
