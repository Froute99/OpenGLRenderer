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
#include <math/Angle.hpp>
#include <glew.h>
#include <iostream>

void Demo::Initialize()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

//void Demo::Update(float /*dt*/)
//{
//	//camera.MoveX(cameraMovement.x);
//	//camera.MoveY(cameraMovement.y);
//	//camera.MoveZ(cameraMovement.z);
//}

void Demo::UpdateWrapper(float dt) noexcept
{
	camera.MoveX(cameraMovement.x);
	camera.MoveY(cameraMovement.y);
	camera.MoveZ(cameraMovement.z);
	Draw::StartDrawing();
	Update(dt);
	Draw::FinishDrawing();
	DrawGUI();
}

void Demo::HandleKeyPress(KeyboardButton key)
{
	key;
}

void Demo::HandleKeyRelease(KeyboardButton key)
{
	key;
}

void Demo::HandleResizeEvent(const int w, const int h)
{
	width = w;
	height = h;
	view.SetViewSize(w, h);
}

void Demo::HandleScrollEvent(float value)
{
	value;
	//const float zoomSpeed = .05f;
	//float newZoom = view.GetZoom() + (scroll_amount * zoomSpeed);
	//newZoom = std::clamp(newZoom, 0.1f, 2.0f);
	//view.SetZoom(newZoom);
}

void Demo::HandleMousePositionEvent(float x, float y)
{
	if (isFirstFrame)
	{
		mousePast[0] = x;
		mousePast[1] = y;
		isFirstFrame = false;
	}

	float dx = (x - mousePast[0]) * 0.005f;
	float dy = (mousePast[1] - y) * 0.005f;
	
	camera.yaw += dx;
	camera.pitch += dy;

	float halfPi = ANGLE::pi / 2.f;

	if (camera.pitch > halfPi)
		camera.pitch = halfPi - 0.00001f;
	else if (camera.pitch < -halfPi)
		camera.pitch = -halfPi + 0.00001f;

	float		cosfY = cosf(camera.pitch);
	vec3<float> newFront = { cosf(camera.yaw) * cosfY, sinf(camera.pitch), sinf(camera.yaw) * cosfY }; // trigonometry : sphere coordinate to cartesian.
	camera.SetFront(Vector3::normalize(newFront));

	mousePast[0] = x;
	mousePast[1] = y;
}

void Demo::HandleMouseEvent(MouseButton mouse)
{
	mouse;
}

void Demo::HandleWindowClose()
{
}

void Demo::HandleFocusEvent(bool focused)
{
	isFocused = focused;
}
