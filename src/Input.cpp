#include "Input.h"
#include "debug.h"

Input::Input(GLFWwindow* window) {
    this->window = window;
}

bool Input::isKeyPressed(const unsigned int key) {
    return glfwGetKey(this->window, key) == GLFW_PRESS;
}

void Input::pollEvents() {
    glfwPollEvents();
}