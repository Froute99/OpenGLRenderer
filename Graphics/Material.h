/*
 *	Author: JeongHak Kim
 *	File_name: Material.h
 *
 *	Include all materials
 *
 *	2026. 03. 04
 */


#pragma once
#include <Math/vec3.hpp>

struct SimpleMaterialPBR
{
	vec3<float> sphereColor{ 1.f, 0.f, 0.f };
	float		roughness = 0.1f;
	float		ambientOcclusion = 0.1f;
	float		metallic = 0.8f;
};
