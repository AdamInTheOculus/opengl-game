EXECUTABLE = game
FLAGS = -Wall
SRC = src/*
INCLUDES = -Iinclude/ -Iinclude/stb_image
LIBS = -lglfw -framework OpenGL

all:
	make build run

build:
	rm bin/*; clear
	g++ ${FLAGS} $(SRC) -o bin/$(EXECUTABLE) $(INCLUDES) $(LIBS)

clean:
	rm bin/$(EXECUTABLE)

run:
	./bin/$(EXECUTABLE)
