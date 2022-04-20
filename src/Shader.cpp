#include "Shader.h"
#include "Debug.h"

Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath)
{
    // Retrieve vertex/fragment shader source from filepath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Ensure ifstream objects can throw exceptions
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // Open files
        vShaderFile.open(vertexFilePath);
        fShaderFile.open(fragmentFilePath);

        // Read file buffer contents into streams
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // Close file buffers
        vShaderFile.close();
        fShaderFile.close();

        // Convert streams into strings
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure &e) {
        DEBUG_ERROR("Failed to load shader. File not successfully read.\n");
        return;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Compile vertex shader
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "Vertex");

    // Compile fragment shader
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "Fragment");

    // Link shader program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);
    glLinkProgram(this->ID);
    checkCompileErrors(this->ID, "Program");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(this->ID);
}

void Shader::use()
{
    glUseProgram(this->ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value); 
}

void Shader::checkCompileErrors(unsigned int shader, const char* type)
{
    int success;
    char infoLog[1024];
    if (strcmp(type, "Program") == 0) {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            DEBUG_ERROR("Shader program failed to link.\n%s\n", infoLog);
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            DEBUG_ERROR("%s shader failed to compile.\n%s\n", type, infoLog);
        }
    }
}
