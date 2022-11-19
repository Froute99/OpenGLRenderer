/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Texture.cpp
 *	
 *	Texture class
 *	
 *	Fall 2019
 *	Oct.07 2019
 */

#include "GL/glew.h"
#include "Image.hpp"
#include "Texture.hpp"
#include "PATH.hpp"

Texture::Texture(Image& image)
{
	LoadFromImage(image);
}


bool Texture::LoadFromPath(const std::filesystem::path& image_path) noexcept
{
	Image image;

	if (!image.LoadFrom(image_path))
	{
		return false;
	}
	return LoadFromImage(image);
}

bool Texture::LoadFromImage(const Image& image) noexcept
{
	if (GetTexturehandle() != 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &textureHandle);
	}

	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.GetWidth(), image.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.GetPixelPointer());
	
	return true;
}
