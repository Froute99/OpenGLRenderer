/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Texture.hpp
 *	
 *	Texture class
 *	
 *	Fall 2019
 *	Oct.07 2019
 */

#pragma once
#include <filesystem>

class Image;
class Color;

class [[nodiscard]] Texture
{
public:
	Texture() = default;
	Texture(Image& image);

	unsigned int GetTexturehandle() const noexcept { return textureHandle; }

	bool LoadFromPath(const std::filesystem::path& image_path, bool useSRGB = false) noexcept;
	bool LoadFromImage(const Image& image) noexcept;
	bool CreateTexture(unsigned int x, unsigned int y) noexcept;
private:
	unsigned int textureHandle;
	
};
