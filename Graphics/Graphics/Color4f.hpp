/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Color4f.hpp
 *	
 *	Color4f class
 *	
 *	Fall 2019
 *	Oct.11 2019
 */

#pragma once
#include <algorithm>

class [[nodiscard]]Color4f
{
public:
	Color4f()
		: r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
	Color4f(float same_intensity, float alpha = 1.0f)
	{
		const float new_color = std::clamp(same_intensity, 0.0f, 1.0f);
		r = new_color;
		g = new_color;
		b = new_color;
		a = alpha;
	}
	Color4f(float red, float green, float blue, float alpha = 1.0f)
	{
		r = std::clamp(red, 0.0f, 1.0f);
		g = std::clamp(green, 0.0f, 1.0f);
		b = std::clamp(blue, 0.0f, 1.0f);
		a = std::clamp(alpha, 0.0f, 1.0f);
	}

	float r, g, b, a;
};

class ColorInChar;

Color4f to4f(ColorInChar eight_bit_color);
