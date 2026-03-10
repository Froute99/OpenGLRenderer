/*
 *	Author: JeongHak Kim
 *
 *	File_name: ShaderManger.h
 *
 *	Shader manager class that helps to load and maintain shaders
 *
 *	2025. 12. 08
 */

#pragma once
#include <string>

struct ShaderFiles
{
	std::string vertexPath;
	std::string fragmentPath;
};

enum class ShaderDefinition
{
	PBR,
	TexturedPBR,
	Phong,
	EquirectangularConversion,
	Irradiance,
	Prefilter,
	BrdfLookupTexture,
	Skybox
};

// Used Singleton Pattern
#include <map>
#include <memory>
#include <ShaderPaths.h>

class Shader;
class ShaderManager
{
public:
	ShaderManager()
	{
		definitions[ShaderDefinition::PBR] = { ShaderPaths::commonVS, ShaderPaths::pbrFS };
		definitions[ShaderDefinition::TexturedPBR] = { ShaderPaths::commonVS, ShaderPaths::texturePbrFS };
		definitions[ShaderDefinition::Phong] = { ShaderPaths::commonVS, ShaderPaths::phongFS };
		definitions[ShaderDefinition::Skybox] = { ShaderPaths::cubemapVS, ShaderPaths::skyboxFS };
		definitions[ShaderDefinition::EquirectangularConversion] = { ShaderPaths::transformVS, ShaderPaths::equirectangular2cubemapFS };
		definitions[ShaderDefinition::Irradiance] = { ShaderPaths::cubemapVS, ShaderPaths::convolutionFS };
		definitions[ShaderDefinition::Prefilter] = { ShaderPaths::transformVS, ShaderPaths::prefilterFS };
		definitions[ShaderDefinition::BrdfLookupTexture] = { ShaderPaths::textureVS, ShaderPaths::brdfLutFS };
	}

	static ShaderManager& GetInstance()
	{
		static ShaderManager instance;
		return instance;
	}

	static Shader* GetShader(ShaderDefinition name);

private:
	ShaderFiles GetFiles(const ShaderDefinition& name) const noexcept
	{
		return definitions.at(name);
	}

	std::map<ShaderDefinition, ShaderFiles>				definitions;
	std::map<ShaderDefinition, std::unique_ptr<Shader>> shaders;

	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;
	ShaderManager(ShaderManager&&) = delete;
	ShaderManager& operator=(ShaderManager&&) = delete;
};
