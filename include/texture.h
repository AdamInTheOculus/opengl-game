#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "debug.h"

class Texture
{
public:

    unsigned int ID;

    Texture(const char* filename)
    {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set texture wrapping/filtering options for currently bound texture.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, colorChannelCount;
        unsigned char* data = stbi_load(filename, &width, &height, &colorChannelCount, 0);
        if(!data) {
            DEBUG_ERROR("Failed to load image [%s]\n", filename);
            return;
        }

        // Generate a texture with loaded image data
        glTexImage2D(
            GL_TEXTURE_2D,    // Texture target. Operations will generate a texture of 
                              // currently bound texture object at the same target
            0,                // Specifies mipmap level. Base level is zero
            GL_RGB,           // Storage format. Our image has only RBG values
            width,            // Set width of resulting texture
            height,           // Set height of resulting texture
            0,                // Always zero (legacy stuff?)
            GL_RGB,           // Format of source image (RGB values)
            GL_UNSIGNED_BYTE, // Data type of source image (char)
            data              // Image data
        );

        // Generate all required mipmaps for currently bound texture.
        glGenerateMipmap(GL_TEXTURE_2D);

        // Clean up image data
        stbi_image_free(data);

        ID = texture;
    }

    ~Texture()
    {
        glDeleteTextures(1, &ID);
    }
};

#endif