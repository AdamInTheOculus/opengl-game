#include "Window.h"
#include "Debug.h"

void frameBufferSizeChangeCallback(GLFWwindow* window, int width, int height);

Window::Window(const unsigned int width, const unsigned int height, const char* title) {
    if(!glfwInit()) { return; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        DEBUG_LOG("OSX GLFW settings enabled.\n");
    #endif

    this->window = glfwCreateWindow(width, height, title, NULL, NULL);
    if(!this->window) {
        DEBUG_ERROR("Failed to create GLFW window.\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(this->window);
    glfwSetFramebufferSizeCallback(this->window, frameBufferSizeChangeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        DEBUG_ERROR("Failed to initialize GLAD.\n");
        return;
    }
}

Window::~Window() {}

bool Window::shouldClose() {
    return glfwWindowShouldClose(this->window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(this->window);
}

void Window::clearScreen() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

GLFWwindow* Window::getGLFWwindow() {
    return this->window;
}

void Window::closeWindow() {
    glfwSetWindowShouldClose(this->window, true);
}


void frameBufferSizeChangeCallback(GLFWwindow* window, int width, int height) {
    DEBUG_LOG("Resizing frame buffer to %dW x %dH.\n", width, height);
    glViewport(0, 0, width, height);
}
