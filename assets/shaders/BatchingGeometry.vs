#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 WorldPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
layout (std140) uniform Matrices
{
    uniform mat4 view;
    uniform mat4 projection;
    uniform vec3 lightPositions;
    uniform vec3 lightColors;
    uniform float lightIntensity;
    uniform vec3 camPos;
};

void main()
{
    WorldPos = aPosition;
    Normal = aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(WorldPos, 1.0);
}