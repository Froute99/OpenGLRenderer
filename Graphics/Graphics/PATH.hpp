/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: PATH.hpp
 *	
 *	All asset files are here
 *	
 *	Fall 2019
 *	Sep.21 2019
 */

#pragma once
#include <filesystem>

namespace PATH
{
	const std::filesystem::path animation_vert = "../assets/animation.vert";
	const std::filesystem::path animation_frag = "../assets/animation.frag";
	
	const std::filesystem::path shape_vert = "../assets/shape.vert";
	const std::filesystem::path shape_frag = "../assets/shape.frag";

	const std::filesystem::path texture_vert = "../assets/texture.vert";
	const std::filesystem::path texture_frag = "../assets/texture.frag";

	const std::filesystem::path icon_image = "../assets/icon.png";

	const std::filesystem::path bitmapfont_fnt = "../assets/bitmapfont.fnt";
	const std::filesystem::path computeShader = "../assets/compute.glsl";
}
