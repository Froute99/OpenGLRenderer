/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: ColorInChar.hpp
 *	
 *	ColorInChar class
 *	
 *	Fall 2019
 *	Oct.13 2019
 */

#pragma once

class [[nodiscard]]ColorInChar
{
public:
	ColorInChar()
		: r(0), g(0), b(0), a(0) {}
	ColorInChar(unsigned char same_intensity, unsigned char alpha = 255)
		: r(same_intensity), g(same_intensity), b(same_intensity), a(alpha) {}
	ColorInChar(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
		: r(r), g(g), b(b), a(a) {}

	unsigned char r, g, b, a;

};