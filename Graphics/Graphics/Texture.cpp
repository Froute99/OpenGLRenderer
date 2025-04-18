/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Texture.cpp
 *
 *	Texture class
 *
 *	Fall 2019
 *	Oct.07 2019
 */

#include "Texture.h"
#include "Image.h"
#include "PATH.h"
#include <glew.h>
#include "stb_image.h"

Texture::Texture(Image& image)
{
	LoadFromImage(image);
}

/* This is the improved verison of texture loader function.
 The original one was little bit wasting memory and performance because
 it assumes all image has 4 component.
 Now this is exactly fit to size of image file. */
bool Texture::LoadFromPath(const std::filesystem::path& image_path, bool useSRGB) noexcept
{
	if (!std::filesystem::exists(image_path))
	{
		return false;
	}

	if (GetTexturehandle() != 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &textureHandle);
	}

	glGenTextures(1, &textureHandle);

	int channel, width, height;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load(image_path.generic_string().c_str(), &width, &height, &channel, 0);
	
	GLenum format = GL_RGBA;
	GLenum internalFormat = GL_RGBA;
	if (channel == 1)
	{
		format = GL_RED;
		internalFormat = GL_RED;
	}
	else if (channel == 3)
	{
		format = GL_RGB;
		internalFormat = useSRGB ? GL_SRGB : GL_RGB;
	}
	else if (channel == 4)
	{
		format = GL_RGBA;
		internalFormat = useSRGB ? GL_SRGB_ALPHA : GL_RGBA;
	}

	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(image);

	return true;
}

// Deprecated. Clean up sometime later.
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
	// Format fixed to GL_RGBA because of assuming an image always has "4" component(RBGA)
	glTexImage2D(GL_TEXTURE_2D, 0, format, image.GetWidth(), image.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.GetPixelPointer());

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
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
