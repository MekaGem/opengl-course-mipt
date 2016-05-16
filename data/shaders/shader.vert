#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textureCoordinates;
layout (location = 3) in vec2 normalCoordinates;
layout (location = 4) in vec3 normal;

out vec3 fragmentColor;
out vec2 fragmentTextureCoordinates;
out vec2 fragmentNormalCoordinates;
out vec3 fragmentNormal;
out vec3 fragmentPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 pos = vec3(position.x, position.y, position.z);
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    fragmentColor = color;
    fragmentTextureCoordinates = textureCoordinates;
    fragmentNormalCoordinates = normalCoordinates;
    fragmentNormal = normal;
    fragmentPosition = position;
}