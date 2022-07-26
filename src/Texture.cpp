#include "Texture.h"
#include "Debug.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int Texture::count = 0;

Texture::Texture(const char* filename, GLenum imageFormat)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    this->ID = texture;
    this->instanceID = count++;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, colorChannelCount;
    unsigned char* imageData = stbi_load(filename, &width, &height, &colorChannelCount, 0);
    if(!imageData) {
        DEBUG_ERROR("Failed to load image from file [%s]", filename);
        return;
    }

    glTexImage2D(
        // Specify texture target. 
        // Operations will generate a texture of the currently bound texture object at the same target.
        GL_TEXTURE_2D,
        0,                // Specifies mipmap level. Base level is zero
        imageFormat,      // Storage format. (RGB, RGBA values)
        width,            // Set width of resulting texture
        height,           // Set height of resulting texture
        0,                // Always zero (legacy stuff?)
        imageFormat,      // Format of source image (RGB, RGBA values)
        GL_UNSIGNED_BYTE, // Data type of source image (char)
        imageData
    );

    stbi_image_free(imageData);
    DEBUG_LOG("Success loading [%s]!", filename);
}

Texture::~Texture()
{
    glDeleteTextures(1, &this->ID);
}

void Texture::use()
{
    glActiveTexture(GL_TEXTURE0 + this->instanceID);
    glBindTexture(GL_TEXTURE_2D, this->ID);
}

/**
 * @brief 
 * 
 */
void Texture::enableMipmaps()
{
    glGenerateMipmap(GL_TEXTURE_2D);
}