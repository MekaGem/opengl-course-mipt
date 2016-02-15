#version 330 core

in vec3 vertexColor;
uniform vec4 ourColor;

out vec4 color;

void main() {
    // color = vec4(vertexColor, 1.0f);
    color = vec4(vertexColor, 1.0f);
}
