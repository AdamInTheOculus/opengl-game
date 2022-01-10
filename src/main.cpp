#include <iostream>
#include "debug.h"

#define DEBUG 1

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window);
void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const int WINDOW_HEIGHT = 720;
const int WINDOW_WIDTH = 1280;

int main() {
    glfwSetErrorCallback(error_callback);

    /**
     * GLFW provides a simple API for creating windows, contexts and surfaces, receiving input and events.
     * Other alternatives are SDL, GLUT, SFML, etc.
    **/ 
    GLFWwindow* window;

    if(!glfwInit()) {
        return -1;
    }

    /**
     * Setting OpenGL version to 3.3
    **/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        DEBUG_LOG("Setting OSX GLFW settings.\n");
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Untitled 2D Game", NULL, NULL);
    if(!window) {
        DEBUG_ERROR("Failed to create GLFW window.\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        DEBUG_ERROR("Failed to initialize GLAD.\n");
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window)) {

        // Process input ...
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
           glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        // Rendering commands ...
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Check/call events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    DEBUG_LOG("Closing application. Clean up complete.\n");
    return 0;
}

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    DEBUG_LOG("Resizing frame buffer to %dW x %dH.\n", width, height);
    glViewport(0, 0, width, height);
}