#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 WorldPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// uniform mat3 normalMatrix;

void main()
{
    WorldPos = vec3(model * vec4(aPosition, 1.0));
    // Normal = normalMatrix * aNormal;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}