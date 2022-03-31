all:
	rm bin/*; clear
	g++ -DDEBUG=1 -Wall src/* -o bin/game -Iinclude/ -Iinclude/stb_image -lglfw -framework OpenGL

build:
	rm bin/*; clear
	g++ -Wall src/* -o bin/game -Iinclude/ -Iinclude/stb_image -lglfw -framework OpenGL

memorycheck:
	rm bin/*; clear
	g++ -DDEBUG=1 -Wall -g src/* -o bin/game -Iinclude/ -Iinclude/stb_image -lglfw -framework OpenGL
	leaks -atExit -- ./bin/game

clean:
	rm bin/game

run:
	./bin/game
