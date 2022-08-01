#include "Window.h"
#include "Debug.h"

GLFWwindow* Window::window = nullptr;
void frameBufferSizeChangeCallback(GLFWwindow* window, int width, int height);

Window::Window(const unsigned int width, const unsigned int height, const char* title) {
    if(!glfwInit()) { return; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        DEBUG_LOG("OSX GLFW settings enabled.");
    #endif

    Window::window = glfwCreateWindow(width, height, title, NULL, NULL);
    if(!this->window) {
        DEBUG_ERROR("Failed to create GLFW window.");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(Window::window);
    glfwSetFramebufferSizeCallback(Window::window, frameBufferSizeChangeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        DEBUG_ERROR("Failed to initialize GLAD.");
        return;
    }

    glEnable(GL_DEPTH_TEST);
}

Window::~Window() {}

bool Window::shouldClose() {
    return glfwWindowShouldClose(Window::window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(Window::window);
}

void Window::clearScreen() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLFWwindow* Window::getWindow() {
    return Window::window;
}

void Window::closeWindow() {
    glfwSetWindowShouldClose(Window::window, true);
}


void frameBufferSizeChangeCallback(GLFWwindow* window, int width, int height) {
    DEBUG_LOG("Resizing frame buffer to %dW x %dH.", width, height);
    glViewport(0, 0, width, height);
}
