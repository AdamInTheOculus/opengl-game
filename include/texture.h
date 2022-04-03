#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Debug.h"

class Texture
{
public:

    unsigned int ID;
    unsigned int instanceCount;

    Texture(unsigned int instanceCount, const char* filename, GLenum imageFormat)
    {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        this->ID = texture;
        this->instanceCount = instanceCount;

        // Set texture wrapping options.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Set texture filtering options.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Set texture mipmap options.
        glGenerateMipmap(GL_TEXTURE_2D);

        int width, height, colorChannelCount;
        unsigned char* data = stbi_load(filename, &width, &height, &colorChannelCount, 0);
        if(!data) {
            DEBUG_ERROR("Failed to load image from file [%s]\n", filename);
            return;
        }

        // Generate a texture with loaded image data
        glTexImage2D(
            GL_TEXTURE_2D,    // Texture target. Operations will generate a texture of 
                              // currently bound texture object at the same target.

            0,                // Specifies mipmap level. Base level is zero
            imageFormat,      // Storage format. (RGB, RGBA values)
            width,            // Set width of resulting texture
            height,           // Set height of resulting texture
            0,                // Always zero (legacy stuff?)
            imageFormat,      // Format of source image (RGB, RGBA values)
            GL_UNSIGNED_BYTE, // Data type of source image (char)
            data              // Image data
        );

        stbi_image_free(data);
        DEBUG_LOG("Created texture from file [%s]\n", filename);
    }

    ~Texture()
    {
        glDeleteTextures(1, &ID);
    }

    void use()
    {
        glActiveTexture(GL_TEXTURE0 + this->instanceCount);
        glBindTexture(GL_TEXTURE_2D, this->ID);
    }
};

#endif