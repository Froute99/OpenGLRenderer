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
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STREAM_DRAW);
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
	char* buffer = reinterpret_cast<char*>(glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY));
	if (buffer == nullptr)
	{
		GLenum errCode = glGetError();
		const unsigned char* errMsg = glewGetErrorString(errCode);
		std::cout << "Was nullptr " << errMsg << ", " << errCode << std::endl;
	}
	memcpy(buffer + offset, data, size);
	glUnmapBuffer(GL_UNIFORM_BUFFER);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//offset += size;
}

GUBO::GUBO()
{
	glGenBuffers(1, &handle);
	glBindBuffer(GL_UNIFORM_BUFFER, handle);
	unsigned int size = sizeof(GUD);		// must consider std140 padding style
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//glBindBufferBase(GL_UNIFORM_BUFFER, SLOT_MATRIX, handle);
}

void GUBO::Bind(unsigned int /*shaderHandle*/)
{
	//unsigned int blockIndex = glGetUniformBlockIndex(shaderHandle, "Matrices");
	//glUniformBlockBinding(shaderHandle, blockIndex, SLOT_MATRIX);

	//std::cout << "Matrix slot " << blockIndex << std::endl;

	glBindBufferBase(GL_UNIFORM_BUFFER, SLOT_MATRIX, handle);
}

void GUBO::Update(GUD newData)
{
	glBindBuffer(GL_UNIFORM_BUFFER, handle);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GUD), &newData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
