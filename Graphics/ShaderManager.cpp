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
#include <glew.h>

Shader* ShaderManager::GetShader(ShaderDefinition name)
{
	ShaderManager& instance = ShaderManager::GetInstance();
	// change this to find logic

	std::map<ShaderDefinition, std::unique_ptr<Shader>>::iterator it;

	if (it = instance.shaders.find(name); it != instance.shaders.end())
	{
		return it->second.get();
	}
	auto uniqeuePtr = std::make_unique<Shader>(instance.definitions[name].vertexPath, instance.definitions[name].fragmentPath);
	auto pair = instance.shaders.emplace(name, std::move(uniqeuePtr));
	return pair.first->second.get();
}
