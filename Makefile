all:
	rm game; clear
	g++ -Wall main.cpp -o game -lglfw -framework OpenGL


clean:
	rm game
