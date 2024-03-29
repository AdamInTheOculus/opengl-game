#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Input {
public:

    Input(GLFWwindow* window);
    bool isKeyPressed(const unsigned int key);
    void processInput();
    void pollEvents();

private:
    GLFWwindow* window;
};

#endif