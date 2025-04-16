#version 330 core
in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D normalMap;
// uniform sampler2D aoMap;

// lights
// uniform vec3 lightPositions[4];
// uniform vec3 lightColors[4];
uniform vec3 lightPositions;
uniform vec3 lightColors;

uniform vec3 camPos;

const float PI = 3.14159265359;
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anyways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N  = normalize(Normal);
    vec3 T  = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
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
float GeometrySchlickBeckmann(float NdotX, float roughness)
{
    // float k = (roughness * roughness) / 2;
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

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
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{
    vec3  albedo    = texture(albedoMap, TexCoords).rgb;    // guaranteed 
    float metallic  = texture(metallicMap, TexCoords).r;
    float roughness = texture(roughnessMap, TexCoords).r;
    // float ao = texture(aoMap, TexCoords).r;
    float ao = 0.1;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(camPos - WorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    // vec3 F0 = vec3(0.04);
    vec3 F0 = vec3(0.04);
    if (metallic > 0.5) F0 = albedo;
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    // for(int i = 0; i < 4; ++i)
    {
        // calculate per-light radiance
        // vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 L = normalize(lightPositions - WorldPos);
        vec3 H = normalize(V + L);
        // float distance = length(lightPositions[i] - WorldPos);
        float distance = length(lightPositions - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        // vec3 radiance = lightColors[i] * attenuation;
        vec3 radiance = lightColors * attenuation;

        // Cook-Torrance BRDF
        float D = DistributionGGX(N, H, roughness);   
        float G = GeometrySmith(N, V, L, roughness);      
        vec3  F = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

        vec3  numerator    = D * G * F; 
        float NdotV = max(dot(N, V), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float denominator = 4.0 * NdotV * NdotL + 0.0001; // + 0.0001 to prevent divide by zero
        vec3  specular = numerator / denominator;

        vec3 kS = F;                    // kS is equal to Fresnel
        vec3 kD = vec3(1.0) - kS;       // By the energy conservation, kD = 1.0 - kS.

        // multiply kD by the inverse metalness such that only non-metals
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - max(metallic, 0.001);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }

    // this ambient light will be changed when IBL is implemented.
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // HDR tonemapping
    // didn't applied HDR here

    // gamma correct
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}