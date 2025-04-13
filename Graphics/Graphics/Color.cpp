/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Color.cpp
 *	
 *	Implementation of Color4f and ColorInChar
 *	
 *	Fall 2019
 *	Oct.13 2019
 */

#include "Color4f.h"
#include "ColorInChar.h"

Color4f to4f(ColorInChar eight_bit_color)
{
	Color4f color4f;
	color4f.r = static_cast<float>(eight_bit_color.r) / static_cast<float>(255);
	color4f.g = static_cast<float>(eight_bit_color.g) / static_cast<float>(255);
	color4f.b = static_cast<float>(eight_bit_color.b) / static_cast<float>(255);
	color4f.a = static_cast<float>(eight_bit_color.a) / static_cast<float>(255);
	return color4f;
}