#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture
{
public:

    unsigned int ID;
    unsigned int instanceCount;

    Texture(unsigned int instanceCount, const char* filename, GLenum imageFormat);
    ~Texture();

    void use();
};

#endif