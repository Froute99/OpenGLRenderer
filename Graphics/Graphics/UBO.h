/*
 *	Author: JeongHak Kim
 *	File_name: UBO.h
 *
 *	Uniform Buffer Object
 *
 *	2025. 07. 14
 */

#pragma once
#include "Shader.h"

static unsigned int globalBindingPoint = 0;

class UBO
{
public:
	UBO(int size);
	void BindTo(const unsigned int shaderHandle, const char* uniformName) noexcept;
	void WriteData(unsigned int size, const void* data);

//private:
	unsigned int handle;
	unsigned int bindingPoint = 0;
	unsigned int offset = 0;

};
