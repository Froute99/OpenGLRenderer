#version 330 core
layout (location = 0) in vec3 aPosition;

out vec3 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = vec3(aPosition.x, -aPosition.y, aPosition.z);
    vec4 pos = projection * view * vec4(aPosition, 1.0);
    gl_Position = pos.xyww;
}