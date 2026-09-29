#include "resources/texture.hpp"

#include "stb_image.h"

#include <glad/glad.h>
#include <iostream>

Texture::Texture(const char* texture_path)
{
    int width;
    int height;
    int channel_count;

    // Load file into RGBA format pixel array
    unsigned char* data = stbi_load(texture_path, &width, &height, &channel_count, 4);

    if (!data) {
        std::cerr << "Failed to load image: " << stbi_failure_reason() << std::endl;
        return;
    }

    // Generate texture object
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Load pixel array data into OpenGL texture
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Unbind the texture and free the pixel array buffer
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

Texture::~Texture()
{
    if (ID == 0) {
        return;
    }

    glDeleteTextures(1, &ID);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, ID);
}
