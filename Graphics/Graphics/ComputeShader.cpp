#include "ComputeShader.h"
#include "Shader.h"
#include "PATH.hpp"
#include <iostream>
#include <fstream>
#include <glew.h>

void ComputeShader::Initialize(unsigned int x, unsigned int y)
{
	LoadShader(PATH::computeShader);
	CreateTexture(x, y);
	SetWorkGroupSize({ x, y });
}

bool ComputeShader::LoadShader(const std::filesystem::path& path) noexcept
{
	const std::string shaderString = ShaderHelper::ReadSourceFrom(path);
	const char* shaderSource = shaderString.c_str();

	const GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, &shaderSource, NULL);
	glCompileShader(computeShader);
	ShaderHelper::CheckCompileErrors(computeShader, "Compute Shader");

	GLuint programID = glCreateProgram();
	glAttachShader(programID, computeShader);
	glLinkProgram(programID);
	ShaderHelper::CheckCompileErrors(programID, "Compute Shader Linkage");

	glDetachShader(programID, computeShader);
	glDeleteShader(computeShader);

	if (handle != 0)
	{
		glDeleteProgram(handle);
	}

	handle = programID;

	return true;
}

void ComputeShader::Use()
{
	glUseProgram(handle);
}

void ComputeShader::BindTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
}

void ComputeShader::Dispatch(unsigned int x, unsigned int y, unsigned int z)
{
	glDispatchCompute(x, y, z);
}

void ComputeShader::Wait()
{
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	//glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void ComputeShader::Run()
{
	Use();
	BindTexture();
	Dispatch(workSize.x, workSize.y, 1);

	// don't know how this works, but it ensure image writing finished before read.
	Wait();
}

void ComputeShader::CreateTexture(unsigned int x, unsigned int y)
{
	if (textureHandle != 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &textureHandle);
	}

	glGenTextures(1, &textureHandle);
	glActiveTexture(GL_TEXTURE0);			// this is useful when binding multiple textures to one shader program
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, x, y, 0, GL_RED, GL_FLOAT, NULL);

	glBindImageTexture(0, textureHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
}
