#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    vec4 color = texture(ourTexture, TexCoord);

    if (color.a == 0.0)
        discard;
    FragColor = color;
}