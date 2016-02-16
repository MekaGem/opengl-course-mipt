#include "shader.hpp"

Shader::Shader(const std::string &vertexShaderName, const std::string &fragmentShaderName) :
        vertexShaderName(vertexShaderName),
        fragmentShaderName(fragmentShaderName) {

    GLint success;
    const int logSize = 512;
    GLchar infoLog[logSize];
    const char *c_str;

    // Creating vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::ifstream vertexShaderIStream(vertexShaderName);
    std::string vertexShaderContent(
            (std::istreambuf_iterator<char>(vertexShaderIStream)),
            (std::istreambuf_iterator<char>())
    );
    c_str = vertexShaderContent.c_str();
    glShaderSource(vertexShader, 1, &c_str, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, logSize, NULL, infoLog);
        std::cout << "Vertex shader compilation fail!\n" << infoLog << std::endl;
    }

    // Creating fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::ifstream fragmentShaderIStream(fragmentShaderName);
    std::string fragmentShaderContent(
            (std::istreambuf_iterator<char>(fragmentShaderIStream)),
            (std::istreambuf_iterator<char>())
    );
    c_str = fragmentShaderContent.c_str();
    glShaderSource(fragmentShader, 1, &c_str, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, logSize, NULL, infoLog);
        std::cout << "Fragment shader compilation fail!\n" << infoLog << std::endl;
    }

    // Creating shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, logSize, NULL, infoLog);
        std::cout << "Shader program linking fail!\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
    glUseProgram(shaderProgram);
}

GLuint Shader::get() {
    return shaderProgram;
}
