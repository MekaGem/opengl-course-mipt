#include <iostream>
#include <fstream>
#include <cmath>
#include <random>

//#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "texture.hpp"
#include "quad.hpp"

void errorCallback(int error, const char* description);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint WIDTH = 800, HEIGHT = 600;

int main() {
    glfwSetErrorCallback(errorCallback);

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyCallback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

//    glViewport(0, 0, WIDTH * 2, HEIGHT * 2);

    Shader shader("shaders/shader.vert", "shaders/shader.frag");

    Texture texture("assets/container.jpg");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::vector<GLfloat> verticesV;
    std::vector<GLuint> indicesV;

    std::vector<Quad> quads;
    quads.push_back(Quad(glm::vec3(-0.5, -0.5, 0), glm::vec3(0, -1, 0), glm::vec3(1, 0, 0)));

    for (int index = 0; index < quads.size(); ++index) {
        Quad quad = quads[index];
        quad.update(verticesV);

        indicesV.push_back(GLuint(4 * index + 0));
        indicesV.push_back(GLuint(4 * index + 1));
        indicesV.push_back(GLuint(4 * index + 2));

        indicesV.push_back(GLuint(4 * index + 2));
        indicesV.push_back(GLuint(4 * index + 3));
        indicesV.push_back(GLuint(4 * index + 0));
    }

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticesV.size(), verticesV.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * indicesV.size(), indicesV.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.get());

        shader.use();

        GLfloat timeValue = (GLfloat) glfwGetTime();

        GLfloat xValue = (GLfloat) (sin(timeValue) / 4) + 0.25f;
        GLint xValueLocation = glGetUniformLocation(shader.get(), "xValue");
        glUniform1f(xValueLocation, xValue);

        GLfloat yValue = (GLfloat) (cos(timeValue + 0.2) / 4) + 0.25f;
        GLint yValueLocation = glGetUniformLocation(shader.get(), "yValue");
        glUniform1f(yValueLocation, yValue);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (GLuint) indicesV.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void errorCallback(int error, const char* description) {
    std::cout << "[" << error << "] " << description << std::endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}