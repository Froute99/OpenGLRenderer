/*
 *	Author: JeongHak Kim
 * 
 *	File_name: ShaderManger.cpp
 *
 *	Shader manager class that helps to load and maintain shaders
 *
 *	2025. 12. 08
 */

#include "ShaderManager.h"
#include <Graphics/Shader.h>

Shader* ShaderManager::GetShader(ShaderDefinition name)
{
	ShaderManager& instance = ShaderManager::GetInstance();
	auto [it, isInserted] = instance.shaders.try_emplace(name,
		std::make_unique<Shader>(instance.definitions[name].vertexPath, instance.definitions[name].fragmentPath));
	return it->second.get();
}
