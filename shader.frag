#version 330 core

in vec3 vertexColor;
in vec2 vertexTextureCoordinates;

uniform sampler2D ourTexture;

out vec4 color;

void main() {
    color = texture(ourTexture, vertexTextureCoordinates) * vec4(vertexColor, 1.0f);;
//    color = vec4(vertexColor, 1.0f);
//    color = vec4(vertexTextureCoordinates.x, 1.0f, 1.0f, 1.0f);
}
