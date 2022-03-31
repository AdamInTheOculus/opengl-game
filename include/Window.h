#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:

    Window(const unsigned int width, const unsigned int height, const char* title);
    ~Window();

    void swapBuffers();
    void clearScreen();
    void closeWindow();
    bool shouldClose();
    GLFWwindow* getGLFWwindow();

private:
    /**
     * GLFW provides a simple API for creating windows, contexts and surfaces, receiving input and events.
     * Other alternatives are SDL, GLUT, SFML, etc.
    **/ 
    GLFWwindow* window;
};

#endif