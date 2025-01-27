/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Image.cpp
 *	
 *	Image class that load from image file
 *	
 *	Fall 2019
 *	Oct.07 2019
 */

#include <cassert>
#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image(const std::filesystem::path& source) noexcept
{
	LoadFrom(source);
}

void Image::ResizePixelSize(int image_width, int image_height) noexcept
{
	assert(image_width > 0 || image_height > 0);
	width = image_width;
	height = image_height; 

	pixels.resize((width + 1) * (height + 1));	// To avoid warning
}

bool Image::LoadFrom(const std::filesystem::path& source) noexcept
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load(source.generic_string().c_str(), &width, &height, &channel, 4);

	if (image != nullptr)
	{
		for (int i = 0; i < width * height; ++i)
		{
			ColorInChar color(image[i * 4], image[i * 4 + 1], image[i * 4 + 2], image[i * 4 + 3]);
			pixels.push_back(color);
		}
		return true;
	}
	stbi_image_free(image);
	return false;
}

void Image::SaveToPNG(const std::filesystem::path& file_path) const noexcept
{
	if (!pixels.empty())
	{
		stbi_write_png(file_path.generic_string().c_str(), width, height, 4, GetPixelPointer(), 0);
	}
}

ColorInChar* Image::GetPixelPointer() noexcept
{
	return &pixels.front();
}

const ColorInChar* Image::GetPixelPointer() const noexcept
{
	return &pixels.front();
}

int Image::GetPixelsBufferBytesSize() const noexcept
{
	return int(pixels.size() * sizeof(ColorInChar));
}

void Image::FlipVertically() noexcept
{
	if (!pixels.empty())
	{
		for (int y = 0; y < height / 2; y++)
		{
			for (int x = 0; x < width; x++)
			{
				ColorInChar temp = pixels.at(y * width + x);
				pixels.at(y * width + x) = pixels.at((height - y) * width + x);
				pixels.at((height - y) * width + x) = temp;
			}
		}
	}
}