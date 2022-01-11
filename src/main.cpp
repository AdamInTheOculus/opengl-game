#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>
#include "debug.h"

#define DEBUG 1

void processInput(GLFWwindow* window);
void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
unsigned int create_shader_program(const char* vertexShaderSource, const char* fragmentShaderSource);
void handle_input(GLFWwindow* window);

const int WINDOW_HEIGHT = 720;
const int WINDOW_WIDTH = 1280;

const float vertices[] = {
    0.5f,  0.5f, 0.0f,   // top right
    0.5f, -0.5f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        DEBUG_ERROR("Failed to initialize GLAD.\n");
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    DEBUG_LOG("Maximum number of vertex attributes is %d.\n", nrAttributes);

    // =====================================
    // == Graphics Setup / Initialization ==
    // =====================================
    const char* vertexShaderSource = "\n"
        "#version 410 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.y, 1.0);\n"
        "}\n"
    ;

    const char* fragmentShaderSource = "\n"
        "#version 410 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 customColor; // we set this variable in the OpenGL code.\n"
        "void main()\n"
        "{\n"
        "   FragColor = customColor;\n"
        "}\n"
    ;

    unsigned int shaderProgram = create_shader_program(vertexShaderSource, fragmentShaderSource);

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

    // Link vertex attributes.
    glVertexAttribPointer(
        0,                 // Location ID of vertex attribute to configure (defined in vertex shader).
        3,                 // Size of vertex attribute. Vertex attribute is vec3 so 3 values.
        GL_FLOAT,          // Type of vertex attribute.
        GL_FALSE,          // Normalize data? No, not relevant right now.
        3 * sizeof(float), // Stride - space between consecutive vertex attributes.
        (void*)0           // Offset position where data begins in buffer.
    );

    // Clean up bound buffer / vertex array.
    glEnableVertexAttribArray(0);
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

        // draw our first triangle
        glUseProgram(shaderProgram);

        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "customColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

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
    glDeleteProgram(shaderProgram);
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

unsigned int create_shader_program(const char* vertexShaderSource, const char* fragmentShaderSource) {
    // Create and compile vertex shader.
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check if vertex compilation was successful.
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        DEBUG_ERROR("Vertex shader failed to compile.\n%s\n", infoLog);
        return -1;
    }

    // Create and compile fragment shader.
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check if fragment compilation was successful.
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        DEBUG_ERROR("Fragment shader failed to compile.\n%s\n", infoLog);
        return -1;
    }

    // Create shader program.
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check if linking shader program was successful.
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        DEBUG_ERROR("Shader program failed to link.\n%s\n", infoLog);
        return -1;
    }

    // Clean up.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
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