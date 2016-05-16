#include <SOIL/SOIL.h>

#include "texture.hpp"

Texture::Texture(const std::string &textureName) : textureName(textureName) {
    unsigned char *image = SOIL_load_image(textureName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::get() {
    return texture;
}

int Texture::getWidth() {
    return width;
}

int Texture::getHeight() {
    return height;
}
