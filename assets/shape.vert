#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 gWorld;

out vec4 outColor;

void main()
{
    gl_Position = gWorld * vec4(position, 1.0);
    outColor = vec4(color, 1.0);
}