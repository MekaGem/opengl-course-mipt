#version 330 core

in vec3 fragmentColor;
in vec2 fragmentTextureCoordinates;
in vec2 fragmentNormalCoordinates;
in vec3 fragmentNormal;
in vec3 fragmentPosition;
in mat3 btn;

uniform sampler2D textureSampler;
uniform sampler2D normalSampler;
uniform vec3 lightSource;
uniform vec3 lookDirection;
uniform int spotlight;

out vec4 color;

void main() {
    vec2 tex = fragmentTextureCoordinates;
    vec3 lightDirection = normalize(lightSource - fragmentPosition);

    vec3 normal = -normalize(2.0 * vec3(texture(normalSampler, fragmentTextureCoordinates)) - 1);
    normal = btn * normal;

    float diff = max(dot(normal, lightDirection), 0.0f);
    vec3 textureColor = vec3(texture(textureSampler, fragmentTextureCoordinates));

    if (spotlight != 0) {
        float diffPower = dot(lookDirection, -lightDirection);
        if (diffPower < 0.9) {
            diff = 0;
        } else {
            diff *= (diffPower - 0.9f) / 0.1f;
        }
    }

    float light = 0.2 + 0.8 * (diff);
    color = vec4(textureColor * light, 1.0f);
}
