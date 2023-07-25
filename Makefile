EXECUTABLE = game
FLAGS = -Wall -std=c++11

# TODO(Adam): Figure out why we need to specify .cpp files explicitly.
SRC = src/*.cpp src/glad/*.cpp

INCLUDES = -Iinclude/ -Iinclude/stb_image -Iinclude/glm -Iinclude/glfw-3.3.8

LIBS = -Llib/glfw-3.3.8 -lglfw -framework OpenGL

all:
	make build run

build:
	rm bin/*; clear
	g++ ${FLAGS} $(SRC) -o bin/$(EXECUTABLE) $(INCLUDES) $(LIBS)

clean:
	rm bin/$(EXECUTABLE)

run:
	./bin/$(EXECUTABLE)
