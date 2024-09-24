/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Texture.cpp
 *
 *	Texture class
 *
 *	Fall 2019
 *	Oct.07 2019
 */

#include "Texture.hpp"
#include "Image.hpp"
#include "PATH.hpp"
#include <glew.h>

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

	GLenum format = GL_RGBA;
	int	   channel = image.GetChannel();
	if (channel == 1)
		format = GL_RED;
	else if (channel == 3)
		format = GL_RGB;
	else if (channel == 4)
		format = GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, format, image.GetWidth(), image.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.GetPixelPointer());
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//stbi_image_free(image);

	return true;
}

bool Texture::CreateTexture(unsigned int x, unsigned int y) noexcept
{
	if (GetTexturehandle() != 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &textureHandle);
	}

	glGenTextures(1, &textureHandle);
	glActiveTexture(GL_TEXTURE0); // this is useful when binding multiple textures to one shader program
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, x, y, 0, GL_RED, GL_FLOAT, NULL);

	glBindImageTexture(0, textureHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);

	return true;
}
