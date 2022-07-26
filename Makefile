EXECUTABLE = game
FLAGS = -Wall

# TODO(Adam): Figure out why we need to specify .cpp files explicitly.
SRC = src/*.cpp src/glad/*.cpp

INCLUDES = -Iinclude/ -Iinclude/stb_image -Iinclude/glm
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
