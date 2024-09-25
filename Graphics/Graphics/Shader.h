/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Shader.h
 *	
 *	Shader class for loading shader
 *	
 *	Fall 2019
 *	Sep.25 2019
 */

#pragma once
#include <filesystem>
#include <Math/mat3.hpp>

namespace ShaderHelper
{
	std::string ReadSourceFrom(const std::filesystem::path& path);
	bool CheckCompileErrors(unsigned int shaderObject, const std::string& errorMsg);
}

class [[nodiscard]] Shader
{
public:
	Shader() noexcept = default;
	Shader(const std::filesystem::path& vertex_source,
		const std::filesystem::path& fragment_source) noexcept;
	bool LoadShaderFrom(const std::filesystem::path& vertex_source,
		const std::filesystem::path& fragment_source) noexcept;
	unsigned GetHandleToShader() const noexcept;

	static void UseShader(const Shader& shader);
	static void UseNothing();

	void SendUniformVariable(const char* variable_name, const int& variable) const noexcept;
	void SendUniformVariable(const char* variable_name, const float& variable) const noexcept;
	void SendUniformVariable(const char* variable_name, const mat3<float>& matrix) const noexcept;
private:
	unsigned int handleToShader = 0;
};
