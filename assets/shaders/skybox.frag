#version 330 core
in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube skybox;

void main()
{
	vec3 color = texture(skybox, TexCoords).rgb;

	// HDR tonemap, gamma correction
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));

	FragColor = vec4(color, 1.0);
}