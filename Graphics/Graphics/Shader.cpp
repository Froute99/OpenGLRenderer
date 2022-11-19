/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Shader.cpp
 *	
 *	Shader class for loading shader
 *	
 *	Fall 2019
 *	Sep.25 2019
 */

#include <iostream>
#include <string>
#include <fstream>
#include "GL\glew.h"
#include "Shader.h"

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

Shader::Shader(const std::filesystem::path& vertex_source,
	const std::filesystem::path& fragment_source) noexcept
{
	LoadShaderFrom(vertex_source, fragment_source);
}

bool Shader::LoadShaderFrom(const std::filesystem::path& vertex_source,
	const std::filesystem::path& fragment_source) noexcept
{
	const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const std::string vert = ReadSourceFrom(vertex_source);
	const std::string frag = ReadSourceFrom(fragment_source);
	
	const char* vertexSource = vert.c_str();
	const char* fragmentSource = frag.c_str();

	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint isVertexCompiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isVertexCompiled);
	if (isVertexCompiled != GL_TRUE)
	{
		GLsizei length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(vertexShader, 1024, &length, message);
		std::cout << "Vertex shader compile failure" << std::endl;
		std::cout << message << std::endl;
	}

	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint isFragmentCompiled;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isFragmentCompiled);
	if (isFragmentCompiled != GL_TRUE)
	{
		GLsizei length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(fragmentShader, 1024, &length, message);
		std::cout << "Fragment shader compile failure" << std::endl;
		std::cout << message << std::endl;
	}

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
	if (handleToShader != 0)
	{
		glDeleteProgram(handleToShader);
	}
	handleToShader = program;
	
	return true;
}

unsigned Shader::GetHandleToShader() const noexcept
{
	return handleToShader;
}

void Shader::UseShader(const Shader& shader)
{
	glUseProgram(shader.GetHandleToShader());
}

void Shader::UseNothing()
{
	glUseProgram(0);
}

void Shader::SendUniformVariable(const char* variable_name, const int& variable) const noexcept
{
	const int location = glGetUniformLocation(handleToShader, variable_name);
	glUniform1i(location, variable);
}

void Shader::SendUniformVariable(const char* variable_name, const float& variable) const noexcept
{
	const int location = glGetUniformLocation(handleToShader, variable_name);
	glUniform1f(location, variable);
}

void Shader::SendUniformVariable(const char* variable_name, const mat3<float>& matrix) const noexcept
{
	const int location = glGetUniformLocation(handleToShader, variable_name);
	float matrix3[] = {
		matrix.elements[0][0], matrix.elements[0][1], matrix.elements[0][2],
		matrix.elements[1][0], matrix.elements[1][1], matrix.elements[1][2],
		matrix.elements[2][0], matrix.elements[2][1], matrix.elements[2][2]
	};
	glUniformMatrix3fv(location, 1, false, matrix3);
}
