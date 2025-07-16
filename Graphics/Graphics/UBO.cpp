/*
 *	Author: JeongHak Kim
 *	File_name: UBO.cpp
 *
 *	Uniform Buffer Object
 *
 *	2025. 07. 14
 */

#include "UBO.h"
#include <glew.h> // glUniform
#include <iostream>

UBO::UBO(int size)
{
	bindingPoint = globalBindingPoint++;
	glGenBuffers(1, &handle);
	glBindBuffer(GL_UNIFORM_BUFFER, handle);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, handle);
}

void UBO::BindTo(const unsigned int shaderHandle, const char* blockName) noexcept
{
	unsigned int blockIndex = glGetUniformBlockIndex(shaderHandle, blockName);
	glUniformBlockBinding(shaderHandle, blockIndex, bindingPoint);
	std::cout << blockName << " index is " << blockIndex << ", binded to point " << bindingPoint << std::endl;
}

void UBO::WriteData(unsigned int offset, unsigned int size, const void* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, handle);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	offset += size;
}
