build/labyrinth: build build/Main.o build/Game.o
	g++ -o build/labyrinth build/Main.o build/Game.o -lSDL2

build:
	mkdir build

build/Main.o: Main.cpp
	g++ -c Main.cpp -o build/Main.o

build/Game.o: Game.cpp
	g++ -c Game.cpp -o build/Game.o
