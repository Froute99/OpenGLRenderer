#version 330 core
in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// material parameters
uniform vec3 albedo;
uniform float roughness;
uniform float ao;
uniform float metallic;

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

uniform float gammaCorrection;

const float PI = 3.14159265359;

// GGX / Trowbridge-Reitz
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// schlick-beckmann model + smith model = schlick-ggx model
float GeometrySchlickBeckmann(float NdotX, float roughness)     // X is either V or L
{
    float k = (roughness * roughness) / 2;

    float nom   = NdotX;
    float denom = NdotX * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickBeckmann(NdotV, roughness);
    float ggx2 = GeometrySchlickBeckmann(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    // clamp using: to prevent black spot.
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);

    // F0 represent base reflectivity, and you might confused with albedo. In here, not the real world,
    // "albedo" represent color of object while "base reflectivity" represent how much lights reflected. (I guess two things are same thing in real world)
    // In case of non-conductor, use 0.04 as F0, but if it is a metal. Using F0 as an albedo.
    vec3 F0 = vec3(0.04);
    if (metallic > 0.5) F0 = albedo;
    F0 = mix(F0, albedo, metallic);     // linear interpolation using the last value as an "alpha"

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < 4; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        // Cook-Torrance BRDF
        float D = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        // clamp part represent F0, and that is between 0 and 1.
        vec3  F = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

        vec3  numerator = D * G * F;
        float NdotV = max(dot(N, V), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float denominator = 4.0 * NdotV * NdotL + 0.0001; // + 0.0001 to prevent divide by zero
        vec3  specular = numerator / denominator;

        vec3 kS = F;                    // kS is equal to Fresnel
        vec3 kD = vec3(1.0) - kS;       // By the energy conservation, kD = 1.0 - kS.

        // metalness is very similar to specular, so the coefficient of diffuse light multiplied by 1 - metallic
        kD *= 1.0 - max(metallic, 0.001);   // to prevent pure metal become absolutly dark

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }

    // this ambient light will be changed when IBL is implemented.
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
 
    // gamma correct
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}