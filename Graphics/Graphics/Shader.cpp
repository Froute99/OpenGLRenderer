/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Shader.cpp
 *
 *	Shader class for loading shader
 *
 *	Fall 2019
 *	Sep.25 2019
 */

#include "Shader.h"
#include <iostream>
#include <string>
#include <fstream>
#include <glew.h>

namespace ShaderHelper
{
	std::string ReadSourceFrom(const std::filesystem::path& path)
	{
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(unsigned(in.tellg()));
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return contents;
		}
		return {};
	}

	bool IsShaderObjectValid(unsigned int shaderObject, const std::string& errorMsg)
	{
		GLint isCompiled;
		glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled != GL_TRUE)
		{
			GLsizei length = 0;
			char message[1024];
			glGetShaderInfoLog(shaderObject, 1024, &length, message);
			std::cout << errorMsg << ": " << message << std::endl;
			return false;
		}
		return true;
	}
}

Shader::Shader(const std::filesystem::path& vertex_source,
	const std::filesystem::path& fragment_source) noexcept
{
	if (!CanLoadShader(vertex_source, fragment_source))
	{
		std::cout << "Shader Compilation Failed\n"
				  << "Check the shader file and its paths\n";
	}
}

Shader::~Shader()
{
	UseNothing();
	glDeleteProgram(handle);
}

bool Shader::CanLoadShader(const std::filesystem::path& vertex_source,
	const std::filesystem::path& fragment_source) noexcept
{
	const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const std::string vert = ShaderHelper::ReadSourceFrom(vertex_source);
	const std::string frag = ShaderHelper::ReadSourceFrom(fragment_source);

	const char* vertexSource = vert.c_str();
	const char* fragmentSource = frag.c_str();

	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	if (!ShaderHelper::IsShaderObjectValid(vertexShader, "Vertex Shader"))
		return false;

	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	if (!ShaderHelper::IsShaderObjectValid(fragmentShader, "Fragment Shader"))
		return false;

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	GLint programLinked;
	glGetProgramiv(program, GL_LINK_STATUS, &programLinked);
	if (programLinked != GL_TRUE)
	{
		GLsizei length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(program, 1024, &length, message);
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (handle != 0)
	{
		glDeleteProgram(handle);
	}
	handle = program;
	UniformBlocksAutoLink();

	return true;
}

unsigned Shader::GetHandleToShader() const noexcept
{
	return handle;
}

void Shader::Use()
{
	glUseProgram(GetHandleToShader());
}

void Shader::UseNothing()
{
	glUseProgram(0);
}

/* Be careful that glGetUniformLocation returns "int" type value.
 * According to khronos documentation: This function returns -1
 * if name does not correspond to an active uniform variable in program,
 * if name starts with the reserved prefix "gl_",
 * or if name is associated with an atomic counter or a named uniform block. */
void Shader::SendUniformVariable(const char* variable_name, const int variable) const noexcept
{
	const int location = glGetUniformLocation(handle, variable_name);
	glUniform1i(location, variable);
}

void Shader::SendUniformVariable(const char* variable_name, const float variable) const noexcept
{
	const int location = glGetUniformLocation(handle, variable_name);
	glUniform1f(location, variable);
}

void Shader::SendUniformVariable(const char* variable_name, const mat3<float>& matrix) const noexcept
{
	const int location = glGetUniformLocation(handle, variable_name);
	glUniformMatrix3fv(location, 1, false, &matrix[0][0]);
}

void Shader::SendUniformVariable(const char* name, const vec3<float>& v) const noexcept
{
	const int location = glGetUniformLocation(handle, name);
	glUniform3fv(location, 1, &v.x);
}

void Shader::SendUniformVariable(const char* name, const mat4<float>& m) const noexcept
{
	const int location = glGetUniformLocation(handle, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]);
}

void Shader::BindTexture(const char* uniformName, const int value, const unsigned int textureHandle) const noexcept
{
	const int location = glGetUniformLocation(handle, uniformName);
	glActiveTexture(GL_TEXTURE0 + value);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glUniform1i(location, value);
}

#include "UBO.h"
void UniformBlockLinkingHelper(unsigned int handle, const std::string& blockName, BindingSlot slot)
{
	// Try to find block with blockName in the shader
	GLuint idx = glGetUniformBlockIndex(handle, blockName.c_str());
	if (idx != GL_INVALID_INDEX)
	{
		// succeed to find the index in the shader, bind the block with the slot
		glUniformBlockBinding(handle, idx, slot);
		std::cout << "\"" << blockName << "\" block found and linked\n";
		return;
	}
	std::cout << "Cannot find block \"" << blockName << "\". Check out the name.\n";
}

void Shader::UniformBlocksAutoLink()
{
	UniformBlockLinkingHelper(handle, "Matrices", SLOT_MATRIX);
}
