#version 330 core
layout (location = 0) in vec3 aPosition;

out vec3 worldPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    worldPos = aPosition;
    vec4 pos = projection * view * vec4(worldPos, 1.0);
    gl_Position = pos.xyww;
}