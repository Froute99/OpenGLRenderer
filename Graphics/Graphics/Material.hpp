/*
 * Rudy Castan
 * Graphics Library
 * CS230
 */
#pragma once
#include <Graphics/Color4f.hpp>
#include <Math/mat3.hpp>
#include <map>
#include <string>


class Shader;
class Texture;
struct [[nodiscard]] texture_uniform
{
	const Texture * texture = nullptr;
	int            textureSlot = 0;
};

struct [[nodiscard]] material
{
	Shader * shader = nullptr;
	std::map<std::string, mat3<float>>         matrix3Uniforms{};
	std::map<std::string, Color4f>         color4fUniforms{};
	std::map<std::string, float>           floatUniforms{};
	std::map<std::string, texture_uniform> textureUniforms{};
};
