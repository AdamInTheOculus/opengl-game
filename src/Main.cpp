#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Debug.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "Input.h"
#include "Camera.h"

void prepare_triangle(unsigned int* vao, unsigned int* vbo, unsigned int* ebo);
void prepare_cube(unsigned int* vao, unsigned int* vbo, unsigned int* ebo);
void error_callback(int error, const char* description);
void mouse_cursor_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const int WINDOW_HEIGHT = 720;
const int WINDOW_WIDTH = 1280;

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -32.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -9.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -6.5f),
    glm::vec3( 1.5f,  2.0f, -14.5f),
    glm::vec3( 1.5f,  0.2f, -11.5f),
    glm::vec3(-1.3f,  1.0f, -8.5f)
};

const int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

float mixVariable = 0.2f;

bool initialMouseCallback = true;
float lastMouseX = WINDOW_WIDTH / 2;
float lastMouseY = WINDOW_HEIGHT / 2;
glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 10.0f);
Camera camera(startPosition);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    glfwSetErrorCallback(error_callback);
    stbi_set_flip_vertically_on_load(true);

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Game");
    Input input(Window::window);

    Texture texture("assets/container.jpg", GL_RGB);
    Texture texture2("assets/awesomeface.png", GL_RGBA);

    Shader shader("shaders/vertex.vs", "shaders/fragment.fs");
    shader.use();
    shader.setInt("customTexture", texture.instanceID);
    shader.setInt("customTexture2", texture2.instanceID);

    unsigned int VAO, VBO, EBO;
    prepare_cube(&VAO, &VBO, &EBO);

    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.window, mouse_cursor_callback);
    glfwSetScrollCallback(window.window, scroll_callback);

    while(!window.shouldClose()) {

        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (input.isKeyPressed(GLFW_KEY_ESCAPE) || input.isKeyPressed(GLFW_KEY_Q)) {
            window.closeWindow();
        }

        if (input.isKeyPressed(GLFW_KEY_1)) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else if (input.isKeyPressed(GLFW_KEY_2)) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else if (input.isKeyPressed(GLFW_KEY_3)) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }

        if(input.isKeyPressed(GLFW_KEY_W))
            camera.handleKeyboardInput(CameraMovement::FORWARD, deltaTime);
        if(input.isKeyPressed(GLFW_KEY_S))
            camera.handleKeyboardInput(CameraMovement::BACKWARD, deltaTime);
        if(input.isKeyPressed(GLFW_KEY_D))
            camera.handleKeyboardInput(CameraMovement::RIGHT, deltaTime);
        if(input.isKeyPressed(GLFW_KEY_A))
            camera.handleKeyboardInput(CameraMovement::LEFT, deltaTime);
        // if(input.isKeyPressed(GLFW_KEY_SPACE))
        //     camera.handleKeyboardInput(CameraMovement::FORWARD, deltaTime);
        // if(input.isKeyPressed(GLFW_KEY_LEFT_CONTROL))
        //     camera.handleKeyboardInput(CameraMovement::FORWARD, deltaTime);

        window.clearScreen();

        texture.use();
        texture2.use();
        shader.use();
        shader.setFloat("mixVariable", mixVariable);

        glm::mat4 view = camera.getViewMatrix();
        shader.setMat4("view", view);

        glm::mat4 projection;
        float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
        projection = glm::perspective(glm::radians(camera.zoom), aspectRatio, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i + 50;

            if(i == 0 || i % 3 == 0) {
                model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            }

            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        window.swapBuffers();
        input.pollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    DEBUG_SUCCESS("Closing application. Clean up complete.");
    return 0;
}

void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    if(initialMouseCallback) {
        lastMouseX = xpos;
        lastMouseY = ypos;
        initialMouseCallback = false;
    }

    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos; // reversed since y-coordinates range from bottom to top

    lastMouseX = xpos;
    lastMouseY = ypos;

    camera.handleMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.handleMouseScroll(static_cast<float>(yoffset));
}


void prepare_triangle(unsigned int* VAO, unsigned int* VBO, unsigned int* EBO)
{
    // ==========================================
    // == Setup OpenGL buffers - VAO, VBO, EBO ==
    // ==========================================
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);

    // Create buffer on GPU and store vertex array data in it.
    // Static draw since it does not change.

    // VAO setup
    glBindVertexArray(*VAO);

    // VBO setup
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO setup
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Specify position attribute
    glVertexAttribPointer(
        0,                 // Location ID of vertex attribute to configure (defined in vertex shader).
        3,                 // Size of vertex attribute. Vertex attribute is vec3 so 3 values.
        GL_FLOAT,          // Type of vertex attribute.
        GL_FALSE,          // Normalize data? No, not relevant right now.
        8 * sizeof(float), // Stride - space between consecutive vertex attributes.
        (void*)0           // Offset position where data begins in buffer.
    );
    glEnableVertexAttribArray(0);

    // Specify color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Specify texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Clean up bound buffer / vertex array.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void prepare_cube(unsigned int* VAO, unsigned int* VBO, unsigned int* EBO)
{
    // ==========================================
    // == Setup OpenGL buffers - VAO, VBO, EBO ==
    // ==========================================
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);

    // Create buffer on GPU and store vertex array data in it.
    // Static draw since it does not change.

    // VAO setup
    glBindVertexArray(*VAO);

    // VBO setup
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO setup
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Specify position attribute
    glVertexAttribPointer(
        0,                 // Location ID of vertex attribute to configure (defined in vertex shader).
        3,                 // Size of vertex attribute. Vertex attribute is vec3 so 3 values.
        GL_FLOAT,          // Type of vertex attribute.
        GL_FALSE,          // Normalize data? No, not relevant right now.
        5 * sizeof(float), // Stride - space between consecutive vertex attributes.
        (void*)0           // Offset position where data begins in buffer.
    );
    glEnableVertexAttribArray(0);

    // Specify texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Clean up bound buffer / vertex array.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void error_callback(int error, const char* description) {
    DEBUG_ERROR("%s", description);
}