/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Screenshot.cpp
 *	
 *	Implementation of screenshot function
 *	
 *	Fall 2019
 *	Dec.05 2019
 */

#pragma once
#include "Image.hpp"

Image ScreenShot(int pixels_width, int pixels_height, int left_x = 0, int bottom_y = 0) noexcept;
