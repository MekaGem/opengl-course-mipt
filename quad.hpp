#ifndef QUAD_HPP
#define QUAD_HPP

#include <GL/glew.h>
#include <glm/detail/func_geometric.hpp>
#include <glm/detail/type_vec3.hpp>


class Quad {
private:
    static const int VERTEX_SIZE = (3 + 3 + 2 + 2 + 3 + 3 + 3);
    float vertices[VERTEX_SIZE * 4];

public:
    Quad(const glm::vec3 &position, const glm::vec3 &left, const glm::vec3 &right, float tx, float ty);

    void update(std::vector<GLfloat> &vertices);
};


#endif //QUAD_HPP
