#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

#include <GL/glew.h>


class Texture {
private:
    GLuint texture;
    int width;
    int height;

public:
    const std::string textureName;

    Texture(const std::string &textureName);

    GLuint get();

    int getWidth();

    int getHeight();
};


#endif //TEXTURE_HPP
