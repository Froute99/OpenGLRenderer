/*
 *	Author: JeongHak Kim
 *	File_name: Material.h
 *
 *	Include all materials
 *
 *	2026. 03. 04
 */


#pragma once
#include "Graphics/Shader.h"
#include <Math/vec3.hpp>

class SimpleMaterialPBR
{
public:
	vec3<float> sphereColor;
	float		roughness;
	float		ambientOcclusion;
	float		metallic;

	void ApplyToShader(Shader* shader)
	{
		// TODO: check the entire sequence of send uniform variable is
		// guaranteed while the proper shader is used
		shader->SendUniformVariable("albedo", sphereColor);
		shader->SendUniformVariable("roughness", roughness);
		shader->SendUniformVariable("ao", ambientOcclusion);
		shader->SendUniformVariable("metallic", metallic);
	}
};
