#version 330 core
out vec4 FragColor;
in vec3 worldPos;

uniform samplerCube envioronmentMap;

const float Pi = 3.14159265359;

void main()
{
    vec3 normal = normalize(worldPos);

    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, normal));
    up = normalize(cross(normal, right));

    const float Two_Pi = Pi * 2.0;
    const float Half_Pi = Pi * 0.5;
    vec3 irradiance = vec3(0.0);
    float stride = 0.025;
    int count = 0;
    for (float phi = 0.0; phi < Two_Pi; phi += stride)
    {
        for (float theta = 0.0; theta < Half_Pi; theta += stride)
        {
            vec3 tangentCoords = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 texCoords = tangentCoords.x * right + tangentCoords.y * up + tangentCoords.z * normal;
            irradiance += texture(envioronmentMap, texCoords).rgb * cos(theta) * sin(theta);
            ++count;
        }
    }
    irradiance = Pi * irradiance * (1.0 / float(count));
    
    FragColor = vec4(irradiance, 1.0);

}