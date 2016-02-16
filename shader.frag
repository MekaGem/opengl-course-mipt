#version 330 core

in vec3 vertexColor;
in vec2 vertexTextureCoordinates;

uniform sampler2D ourTexture;
uniform float xValue;
uniform float yValue;

out vec4 color;

void main() {
    vec2 tex = vertexTextureCoordinates;
    tex.x += xValue;
    tex.y += yValue;
    color = texture(ourTexture, tex) * vec4(vertexColor, 1.0f);
//    color = vec4(vertexColor, 1.0f);
//    color = vec4(vertexTextureCoordinates.x, 1.0f, 1.0f, 1.0f);
}
