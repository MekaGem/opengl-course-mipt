#include <vector>

#include <GL/glew.h>

#include "quad.hpp"

Quad::Quad(const glm::vec3 &position, const glm::vec3 &left, const glm::vec3 &right) {
    std::vector<glm::vec3> vertices;
    vertices.push_back(position);
    vertices.push_back(position + left);
    vertices.push_back(position + left + right);
    vertices.push_back(position + right);

    for (int index = 0; index < 4; ++index) {
        glm::vec3 v = vertices[index];

        Quad::vertices[VERTEX_SIZE * index + 0] = v.x;
        Quad::vertices[VERTEX_SIZE * index + 1] = v.y;
        Quad::vertices[VERTEX_SIZE * index + 2] = v.z;

        Quad::vertices[VERTEX_SIZE * index + 3] = 0.0f;
        Quad::vertices[VERTEX_SIZE * index + 4] = 1.0f;
        Quad::vertices[VERTEX_SIZE * index + 5] = 0.0f;

        Quad::vertices[VERTEX_SIZE * index + 6] = index < 2 ? 0.0f : 1.0f;
        Quad::vertices[VERTEX_SIZE * index + 7] = index > 0 && index < 3 ? 1.0f : 0.0f;
    }
}

void Quad::update(std::vector<GLfloat> &vertices) {
    vertices.insert(vertices.end(), Quad::vertices, Quad::vertices + VERTEX_SIZE * 4);
}
