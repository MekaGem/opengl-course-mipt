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
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "texture.hpp"
#include "quad.hpp"
#include "map.hpp"

void errorCallback(int error, const char *description);

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

const GLuint WIDTH = 800, HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3(1.5f, 0.0f, -1.5f);
const glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float rotation = 0;

bool keys[1024];

enum Direction {
    LEFT = 0,
    FORWARD = 1,
    RIGHT = 2,
    BACKWARD = 3
};

glm::vec3 WALL_SHIFT[] = {
        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, -1),
        glm::vec3(1, 0, -1),
        glm::vec3(1, 0, 0),
};

glm::vec3 WALL_DIRECTION[] = {
        glm::vec3(0, 0, -1),
        glm::vec3(1, 0, 0),
        glm::vec3(0, 0, 1),
        glm::vec3(-1, 0, 0),
};

int dx[] = {-1, 0, 1, 0};

int dy[] = {0, 1, 0, -1};

int initWindow(GLFWwindow *&window) {
    glfwSetErrorCallback(errorCallback);

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

void createBuffers(GLuint &VBO, GLuint &VAO, GLuint &EBO, const std::vector<GLfloat> &vertices,
                   const std::vector<GLuint> &indices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * indices.size(), indices.data(), GL_STATIC_DRAW);

    GLsizei length = (3 + 3 + 2 + 3) * sizeof(GLfloat);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, length, (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, length, (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, length, (GLvoid *) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, length, (GLvoid *) (8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}

void disposeBuffers(GLuint &VBO, GLuint &VAO, GLuint &EBO) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void update(float delta) {
    glm::vec3 r = glm::rotate(cameraFront, -rotation, cameraUp);

    GLfloat cameraMovementSpeed = 0.6f * delta;
    GLfloat cameraRotationSpeed = 0.4f * delta;
    if(keys[GLFW_KEY_W])
        cameraPos += cameraMovementSpeed * 4 * r;
    if(keys[GLFW_KEY_S])
        cameraPos -= cameraMovementSpeed * 4 * r;
    if(keys[GLFW_KEY_A])
        rotation -= glm::pi<float>() * cameraRotationSpeed;
    if(keys[GLFW_KEY_D])
        rotation += glm::pi<float>() * cameraRotationSpeed;
}

int main() {
    std::string dataRoot;
#ifdef DATA_ROOT
    dataRoot = DATA_ROOT;
#else
    std::cerr << "No DATA_ROOT path found" << std::endl;
    return -1;
#endif

    GLFWwindow *window = nullptr;

    if (initWindow(window)) {
        return -1;
    }

    Shader shader(dataRoot + "/data/shaders/shader.vert", dataRoot + "/data/shaders/shader.frag");

    Texture texture(dataRoot + "/data/assets/wall.jpg");

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    std::vector<Quad> quads;

    Map map(100, 100);
    for (int x = 0; x < map.getWidth(); ++x) {
        for (int y = 0; y < map.getHeight(); ++y) {
            if (map.isPassable(x, y)) {
                quads.push_back(Quad(glm::vec3(x, 0, -y), glm::vec3(1, 0, 0), glm::vec3(0, 0, -1)));
                quads.push_back(Quad(glm::vec3(x, 1, -y), glm::vec3(0, 0, -1), glm::vec3(1, 0, 0)));
            }
            for (int direction = 0; direction < 4; ++direction) {
                int cx = x + dx[direction];
                int cy = y + dy[direction];
                if (map.isPassable(x, y) && !map.isPassable(cx, cy)) {
                    quads.push_back(Quad(
                            glm::vec3(x, 0, -y) + WALL_SHIFT[direction],
                            glm::vec3(0, 1, 0),
                            WALL_DIRECTION[direction]
                    ));
                }
            }
        }
    }

    for (int index = 0; index < quads.size(); ++index) {
        Quad quad = quads[index];
        quad.update(vertices);

        indices.push_back(GLuint(4 * index + 0));
        indices.push_back(GLuint(4 * index + 1));
        indices.push_back(GLuint(4 * index + 2));

        indices.push_back(GLuint(4 * index + 2));
        indices.push_back(GLuint(4 * index + 3));
        indices.push_back(GLuint(4 * index + 0));
    }

    GLuint VBO, VAO, EBO;
    createBuffers(VBO, VAO, EBO, vertices, indices);

    glm::mat4 projection;
    projection = glm::perspective(45.0f, WIDTH / (float) HEIGHT, 0.1f, 100.0f);

    float time = (float) glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        float cTime = (float) glfwGetTime();
        float delta = cTime - time;
        time = cTime;

        update(delta);

        glClearColor(117 / 255.0f, 187 / 255.0f, 253 / 255.0f, 1.0f);
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

        glm::mat4 model;
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, -cameraPos);

        GLint modelLocation = glGetUniformLocation(shader.get(), "model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

        glm::mat4 view;
        glm::vec3 eye(0.0f, 0.5f, 0.0f);
        view = glm::lookAt(eye, eye + cameraFront, cameraUp);

        GLint viewLocation = glGetUniformLocation(shader.get(), "view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

        GLint projectionLocation = glGetUniformLocation(shader.get(), "projection");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (GLuint) indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    disposeBuffers(VBO, VAO, EBO);

    glfwTerminate();
    return 0;
}

void errorCallback(int error, const char *description) {
    std::cout << "[" << error << "] " << description << std::endl;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (action == GLFW_PRESS) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}
