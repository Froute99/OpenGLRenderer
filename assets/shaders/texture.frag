#version 330 core
in vec3 fragPos;
in vec3 vertexNormal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform sampler2D textureDiffuse;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(vertexNormal);
	vec3 lightDir = normalize(lightPosition - fragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 textureColor = vec3(texture(textureDiffuse, TexCoords));

	vec3 result = (ambient + diffuse) * textureColor;
	FragColor = vec4(result, 1.0);
}