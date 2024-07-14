#include "ComputeShader.h"
#include "GL\glew.h"
#include <iostream>
#include <fstream>

namespace
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
}

bool CheckCompileErrors(GLint isCompiled, GLuint shaderObject, const std::string& errorMsg)
{
	if (isCompiled != GL_TRUE)
	{
		GLsizei length = 0;
		char message[1024];
		glGetShaderInfoLog(shaderObject, 1024, &length, message);
		std::cout << errorMsg << std::endl;
		std::cout << length << " letters error msg\t" << message << std::endl;
		return false;
	}
	return true;
}

bool ComputeShader::LoadShader(const std::filesystem::path& path) noexcept
{
	const std::string shaderString = ReadSourceFrom(path);
	const char* shaderSource = shaderString.c_str();

	const GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, &shaderSource, NULL);
	glCompileShader(computeShader);
	GLint isCompiled;
	glGetShaderiv(computeShader, GL_COMPILE_STATUS, &isCompiled);
	CheckCompileErrors(isCompiled, computeShader, "Compute shader compile failure");

	GLuint programID = glCreateProgram();
	glAttachShader(programID, computeShader);
	glLinkProgram(programID);
	isCompiled;
	glGetShaderiv(programID, GL_COMPILE_STATUS, &isCompiled);
	CheckCompileErrors(isCompiled, programID, "Program error");

	glDetachShader(programID, computeShader);
	glDeleteShader(computeShader);

	if (handle != 0)
	{
		glDeleteProgram(handle);
	}

	handle = programID;

	return true;
}

void ComputeShader::Use() noexcept
{
	//if (handle == 0)
	//{
	//	std::cout << "Compute shader use failure. Invalid handle" << std::endl;
	//	return;
	//}
	glUseProgram(handle);
}
