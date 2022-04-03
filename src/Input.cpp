#include "Input.h"
#include "Debug.h"

Input::Input(GLFWwindow* window) {
    this->window = window;
}

bool Input::isKeyPressed(const unsigned int key) {
    return glfwGetKey(this->window, key) == GLFW_PRESS;
}

void Input::pollEvents() {
    glfwPollEvents();
}