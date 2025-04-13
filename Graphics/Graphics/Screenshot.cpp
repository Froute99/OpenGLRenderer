/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Screenshot.cpp
 *	
 *	Implementation of screenshot function
 *	
 *	Fall 2019
 *	Dec.05 2019
 */

#include "Screenshot.h"
#include <glew.h>

Image ScreenShot(int pixels_width, int pixels_height, int left_x, int bottom_y) noexcept
{
	Image screenshot;
	glReadBuffer(GL_BACK);
	screenshot.ResizePixelSize(pixels_width, pixels_height);
	if (glReadnPixels != nullptr)
	{
		glReadnPixels(left_x, bottom_y, pixels_width, pixels_height, GL_RGBA, GL_UNSIGNED_BYTE, screenshot.GetPixelsBufferBytesSize(), screenshot.GetPixelPointer());
	}
	else
	{
		glReadPixels(left_x, bottom_y, pixels_width, pixels_height, GL_RGBA, GL_UNSIGNED_BYTE, screenshot.GetPixelPointer());
	}
	screenshot.FlipVertically();

	return screenshot;
}
