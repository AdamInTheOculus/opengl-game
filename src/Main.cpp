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

void prepare_triangle(unsigned int* vao, unsigned int* vbo, unsigned int* ebo);
void error_callback(int error, const char* description);

const int WINDOW_HEIGHT = 720;
const int WINDOW_WIDTH = 1280;

const float vertices[] = {
    // positions         // colours          // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

const int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

float mixVariable = 0.2f;

int main() {
    glfwSetErrorCallback(error_callback);
    stbi_set_flip_vertically_on_load(true);

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "2D Platformer");
    Input input(Window::window);

    Texture texture("assets/container.jpg", GL_RGB);
    Texture texture2("assets/awesomeface.png", GL_RGBA);

    Shader shader("shaders/vertex.vs", "shaders/fragment.fs");
    shader.use();
    shader.setInt("customTexture", texture.instanceID);
    shader.setInt("customTexture2", texture2.instanceID);

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    DEBUG_LOG("Vector (%.0f,%.0f,%.0f)", vec.x, vec.y, vec.z);

    unsigned int VAO, VBO, EBO;
    prepare_triangle(&VAO, &VBO, &EBO);

    while(!window.shouldClose()) {
        if (input.isKeyPressed(GLFW_KEY_ESCAPE)|| input.isKeyPressed(GLFW_KEY_Q) || input.isKeyPressed(GLFW_KEY_SPACE)) {
            window.closeWindow();
        }

        if (input.isKeyPressed(GLFW_KEY_1)) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else if (input.isKeyPressed(GLFW_KEY_2)) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else if (input.isKeyPressed(GLFW_KEY_3)) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }

        if (input.isKeyPressed(GLFW_KEY_UP)) {
            mixVariable += 0.01f;
            if (mixVariable > 1.0f) {
                mixVariable = 1.0f;
            }
        } else if (input.isKeyPressed(GLFW_KEY_DOWN)) {
            mixVariable -= 0.01f;
            if (mixVariable < 0.0f) {
                mixVariable = 0.0f;
            }
        }

        window.clearScreen();

        texture.use();
        texture2.use();
        shader.use();
        shader.setFloat("mixVariable", mixVariable);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

void prepare_triangle(unsigned int* VAO, unsigned int* VBO, unsigned int* EBO)
{
    // ==========================================
    // == Setup OpenGL buffers - VAO, VBO, EBO ==
    // ==========================================
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);

    // Create buffer on GPU and store vertex array data in it.
    // Static draw since it's not change at all.

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

void error_callback(int error, const char* description) {
    DEBUG_ERROR("%s", description);
}