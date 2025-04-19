/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	
 *	File_name: Demo.cpp
 *	
 *	Graphic demo main file
 *	
 *	Nov.28 2019
 *******************************************************/

#include "Demo.h"
#include <Graphics/Draw.h>
#include <glew.h> // glUniform

void Demo::Initialize()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

void Demo::HandleResizeEvent(const int& new_width, const int& new_height)
{
	new_width, new_height;
	//view.SetViewSize(new_width, new_height);
}

void Demo::HandleScrollEvent(float scroll_amount)
{
	scroll_amount;
	//const float zoomSpeed = .05f;
	//float newZoom = view.GetZoom() + (scroll_amount * zoomSpeed);
	//newZoom = std::clamp(newZoom, 0.1f, 2.0f);
	//view.SetZoom(newZoom);
}

void Demo::HandleFocusEvent(bool focused)
{
	isFocused = focused;
}
