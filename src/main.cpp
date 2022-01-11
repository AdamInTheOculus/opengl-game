#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

#include "debug.h"
#include "shader.h"

#define DEBUG 1

void processInput(GLFWwindow* window);
void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
unsigned int create_shader_program(const char* vertexShaderSource, const char* fragmentShaderSource);
void handle_input(GLFWwindow* window);

const int WINDOW_HEIGHT = 720;
const int WINDOW_WIDTH = 1280;

const float vertices[] = {
    // positions         // colours  
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // bottom right
   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, // bottom left
   -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f  // top left 
};

const int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

// const float vertices1[] = {
//     // triangle 1
//     0.5f,  0.5f, 0.0f,   // top right
//     0.5f, -0.5f, 0.0f,   // bottom right
//     -0.5f, -0.5f, 0.0f  // bottom left
// };

// const float vertices2[] = {
//     // triangle 2
//     0.5f,  0.5f, 0.0f,   // top right
//     -0.5f, -0.5f, 0.0f,  // bottom left
//     -0.5f, 0.5f, 0.0f,   // top left
// };

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        DEBUG_ERROR("Failed to initialize GLAD.\n");
        return -1;
    }

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    DEBUG_LOG("Maximum number of vertex attributes is %d.\n", nrAttributes);

    // unsigned int shaderProgram = create_shader_program(vertexShaderSource, fragmentShaderSource);
    Shader shaderInstance("shaders/vertex.vs", "shaders/fragment.fs");

    // ===================
    // == Creating VAOs ==
    // ===================
    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Create buffer on GPU and store vertex array data in it.
    // Static draw since it's not change at all.
    
    // VAO setup
    glBindVertexArray(VAO);

    // VBO setup
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO setup
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Specify position attribute.
    glVertexAttribPointer(
        0,                 // Location ID of vertex attribute to configure (defined in vertex shader).
        3,                 // Size of vertex attribute. Vertex attribute is vec3 so 3 values.
        GL_FLOAT,          // Type of vertex attribute.
        GL_FALSE,          // Normalize data? No, not relevant right now.
        6 * sizeof(float), // Stride - space between consecutive vertex attributes.
        (void*)0           // Offset position where data begins in buffer.
    );
    glEnableVertexAttribArray(0);

    // Specify color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Clean up bound buffer / vertex array.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


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

        shaderInstance.use();
        // shaderInstance.setFloat("offset", 0.25f);
        // float timeValue = glfwGetTime();
        // float greenValue = sin(timeValue) / 2.0f + 0.5f;
        // shaderInstance.setFloat("customColor", greenValue);

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
    fprintf(stderr, "Error: %s\n", description);
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
}