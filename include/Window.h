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

    static GLFWwindow* getWindow();
    static GLFWwindow* window;

private:

};

#endif