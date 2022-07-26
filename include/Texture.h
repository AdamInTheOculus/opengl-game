#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture
{

private:
    /**
     * @brief Enables mipmaps. Automatically sets texture image quality based on distance to object.
     */
    void enableMipmaps();

public:
    unsigned int ID;
    unsigned int instanceID;

    /**
     * @brief Global count of all current loaded textures.
     */
    static unsigned int count;

    Texture(const char* filename, GLenum imageFormat);
    ~Texture();

    /**
     * @brief Sets the texture to be current active texture. Any subsequent texture operations
     * will be performed on this texture.
     * @returns void
     */
    void use();
};

#endif