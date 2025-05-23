#version 330 core
out vec4 FragColor;

in vec3 vertexNormal;
in vec3 fragPos;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D textureDiffuse1;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(vertexNormal);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 textureColor = vec3(texture(textureDiffuse1, TexCoords));

	vec3 result = (ambient + diffuse) * textureColor;
	FragColor = vec4(result, 1.0);
}