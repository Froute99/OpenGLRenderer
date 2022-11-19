/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Image.hpp
 *	
 *	Image class that load from image file
 *	
 *	Fall 2019
 *	Oct.07 2019
 */

#pragma once
#include <vector>
#include <filesystem>
#include "Color4f.hpp"
#include "ColorInChar.hpp"

class [[nodiscard]] Image
{
public:
	Image() = default;
	Image(const std::filesystem::path & source) noexcept;

	void ResizePixelSize(int image_width, int image_height) noexcept;
	bool LoadFrom(const std::filesystem::path & source) noexcept;
	void SaveToPNG(const std::filesystem::path & file_path) const noexcept;

	int GetWidth() const noexcept;
	int GetHeight() const noexcept;

	ColorInChar* GetPixelPointer() noexcept;
	const ColorInChar* GetPixelPointer() const noexcept;
	int GetPixelsBufferBytesSize() const noexcept;

	void FlipVertically() noexcept;
private:
	int width, height;
	std::vector<ColorInChar> pixels{};
};