#version 330 core
in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform sampler2D textureDiffuse;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPosition - WorldPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 textureColor = vec3(texture(textureDiffuse, TexCoords));

	vec3 result = (ambient + diffuse) * textureColor;
	FragColor = vec4(result, 1.0);
}