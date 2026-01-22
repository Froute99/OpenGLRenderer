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

GlobalUniformBuffer::GlobalUniformBuffer()
{
	glGenBuffers(1, &handle);
	glBindBuffer(GL_UNIFORM_BUFFER, handle);
	unsigned int size = sizeof(GlobalUniformData);		// must consider std140 padding style
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//glBindBufferBase(GL_UNIFORM_BUFFER, SLOT_MATRIX, handle);
}

void GlobalUniformBuffer::Bind(unsigned int /*shaderHandle*/)
{
	//unsigned int blockIndex = glGetUniformBlockIndex(shaderHandle, "Matrices");
	//glUniformBlockBinding(shaderHandle, blockIndex, SLOT_MATRIX);

	glBindBufferBase(GL_UNIFORM_BUFFER, SLOT_MATRIX, handle);
}

void GlobalUniformBuffer::Update(GlobalUniformData newData)
{
	glBindBuffer(GL_UNIFORM_BUFFER, handle);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GlobalUniformData), &newData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
