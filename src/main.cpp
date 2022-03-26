#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

#include "debug.h"
#include "shader.h"
#include "texture.h"

void prepare_triangle(unsigned int* vao, unsigned int* vbo, unsigned int* ebo);
void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void handle_input(GLFWwindow* window);

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

    /**
     * GLFW provides a simple API for creating windows, contexts and surfaces, receiving input and events.
     * Other alternatives are SDL, GLUT, SFML, etc.
    **/ 
    GLFWwindow* window;

    if(!glfwInit()) { return -1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        DEBUG_LOG("OSX GLFW settings enabled.\n");
    #endif

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Untitled 2D Game", NULL, NULL);
    if(!window) {
        DEBUG_ERROR("Failed to create GLFW window.\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        DEBUG_ERROR("Failed to initialize GLAD.\n");
        return -1;
    }


    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    DEBUG_LOG("Maximum number of vertex attributes is %d.\n", nrAttributes);

    unsigned int VAO, VBO, EBO;
    prepare_triangle(&VAO, &VBO, &EBO);

    // ====================
    // == Setup textures ==
    // ====================
    stbi_set_flip_vertically_on_load(true);
    Texture texture(0, "assets/container.jpg", GL_RGB);
    Texture texture2(1, "assets/awesomeface.png", GL_RGBA);

    // ==========================
    // == Setup shader program ==
    // ==========================
    Shader shader("shaders/vertex.vs", "shaders/fragment.fs");
    shader.use();
    shader.setInt("customTexture", texture.instanceCount);
    shader.setInt("customTexture2", texture2.instanceCount);

    while (!glfwWindowShouldClose(window)) {

        // ===================
        // == Process input ==
        // ===================
        handle_input(window);

        // ========================
        // == Rendering commands ==
        // ========================
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        texture.use();
        texture2.use();

        shader.use();
        shader.setFloat("mixVariable", mixVariable);

        // finally, render the triangle
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // ====================================
        // == Swap buffer and poll IO events ==
        // ====================================
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    DEBUG_LOG("Closing application. Clean up complete.\n");
    return 0;
}

void error_callback(int error, const char* description) {
    DEBUG_ERROR("%s\n", description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    DEBUG_LOG("Resizing frame buffer to %dW x %dH.\n", width, height);
    glViewport(0, 0, width, height);
}

void handle_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        mixVariable += 0.01f;
        if(mixVariable > 1.0f) {
            mixVariable = 1.0f;
        }
    } else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        mixVariable -= 0.01f;
        if(mixVariable < 0.0f) {
            mixVariable = 0.0f;
        }
    }
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