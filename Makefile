all:
	rm bin/*; clear
	g++ -Wall src/* -o bin/game -Iinclude -lglfw -framework OpenGL

clean:
	rm bin/game

run:
	./bin/game
