all:
	rm bin/*; clear
	g++ -Wall src/* -o bin/game -Iinclude/ -Iinclude/stb_image -lglfw -framework OpenGL

clean:
	rm bin/game

run:
	./bin/game
