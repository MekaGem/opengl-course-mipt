#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <fstream>
#include <iostream>

#include <GL/glew.h>


class Shader {
private:
    GLuint shaderProgram;
public:
    const std::string vertexShaderName;
    const std::string fragmentShaderName;

    Shader(const std::string &vertexShaderName, const std::string &fragmentShaderName);

    void use();

    GLuint get();
};


#endif //SHADER_HPP
