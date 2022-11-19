/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	
 *	File_name: ShapeDrawingDemo.cpp
 *	
 *	Simple Shape Drawing Demo
 *	
 *	Nov.25 2019
 *******************************************************/

#include <iostream>
#include "glfw3.h"
#include "ShapeDrawingDemo.hpp"
#include "Draw.hpp"
#include "PATH.hpp"
#include "Screenshot.hpp"

void ShapeDrawingDemo::Initialize()
{
	shader.LoadShaderFrom(PATH::shape_vert, PATH::shape_frag);
	const Color4f color{ .8f, .8f, .0f, 1.0f };

	vec2<float> size{ 50.0f };
	rectangle = MESH::create_rectangle({ 0.0f }, size, color);
	rectangleVertices.InitializeWithMeshAndLayout(rectangle, layout);

	line = MESH::create_line({ 100.0f, 0.0f }, { 50.0f, 50.0f }, { -50.0f, -50.0f }, color);
	lineVertices.InitializeWithMeshAndLayout(line, layout);

	quad = MESH::create_quad({ 0.0f, 100.0f }, { -20.f, 25.f }, { -50.f, -25.f }, { 50.f, -25.f }, { 20.f, 25.f }, color);
	quadVertices.InitializeWithMeshAndLayout(quad, layout);

	triangle = MESH::create_triangle({ 100.0f }, { 0.0f, 50.0f }, { -30.f, 0.0f }, { 30.f, 0.0f }, color);
	triangleVertices.InitializeWithMeshAndLayout(triangle, layout);

	circle = MESH::create_ellipse({ 200.0f, 25.0f }, size, 30, color);
	circleVertices.InitializeWithMeshAndLayout(circle, layout);

	ellipse = MESH::create_ellipse({ -200.0f, 25.0f }, { 50.0f, 30.f }, 30, color);
	ellipseVertices.InitializeWithMeshAndLayout(ellipse, layout);

	view.SetViewSize(width, height);
	std::cout << "\t====================================\n";
	std::cout << "\tPress Arrow keys to move the camera\n";
	std::cout << "\tPress Z, X keys to rotate the camera\n";
	std::cout << "\tPress \"Enter\" key to screenshot\n";
	std::cout << "\t====================================\n";

	const mat3<float> cameraToNDC = view.GetCameraToNDCTransform() * camera.WorldToCamera();

	ndc = cameraToNDC * rectangleTransform.GetModelToWorld();
	lineNDC = cameraToNDC * lineTransform.GetModelToWorld();
	quadNDC = cameraToNDC * quadTransform.GetModelToWorld();
	ellipseNDC = cameraToNDC * ellipseTransform.GetModelToWorld();
	triangleNDC = cameraToNDC * triangleTransform.GetModelToWorld();
	circleNDC = cameraToNDC * circleTransform.GetModelToWorld();
}

void ShapeDrawingDemo::Update(float dt)
{
	std::cout << "\r" << dt;

	if (!isFocused)
	{
		Draw::StartDrawing();
		return;
	}

	camera.Rotate(rotationSpeed);
	camera.MoveRight(moveSpeed.x);
	camera.MoveUp(moveSpeed.y);
	
	Draw::StartDrawing();

	Draw::DrawShape({ shader, rectangleVertices, ndc });
	Draw::DrawShape({ shader, lineVertices, lineNDC });
	Draw::DrawShape({ shader, quadVertices, quadNDC });
	Draw::DrawShape({ shader, triangleVertices, ellipseNDC });
	Draw::DrawShape({ shader, circleVertices, triangleNDC });
	Draw::DrawShape({ shader, ellipseVertices, circleNDC });
	
	Draw::FinishDrawing();
}

void ShapeDrawingDemo::ResetCamera()
{
	camera.ResetUp();
}

void ShapeDrawingDemo::HandleResizeEvent(const int& new_width, const int& new_height)
{	
	Demo::HandleResizeEvent(new_width, new_height);
}

void ShapeDrawingDemo::HandleKeyPress(KeyboardButton button)
{
	switch (button)
	{
	case KeyboardButton::Arrow_Right:
		moveSpeed.x = 2.0f;
		break;
	case KeyboardButton::Arrow_Left:
		moveSpeed.x = -2.0f;
		break;
	case KeyboardButton::Arrow_Up:
		moveSpeed.y = 2.0f;
		break;
	case KeyboardButton::Arrow_Down:
		moveSpeed.y = -2.0f;
		break;
	case KeyboardButton::Z:
		rotationSpeed = 0.1f;
		break;
	case KeyboardButton::X:
		rotationSpeed = -0.1f;
		break;
	case KeyboardButton::Enter:
		auto screenshot = ScreenShot(width, height);
		screenshot.SaveToPNG("../ScreenshotTest.png");
		break;
	}
}

void ShapeDrawingDemo::HandleKeyRelease(KeyboardButton button)
{
	switch (button)
	{
	case KeyboardButton::Arrow_Right:
		moveSpeed.x = 0.0f;
		break;
	case KeyboardButton::Arrow_Left:
		moveSpeed.x = 0.0f;
		break;
	case KeyboardButton::Arrow_Up:
		moveSpeed.y = 0.0f;
		break;
	case KeyboardButton::Arrow_Down:
		moveSpeed.y = 0.0f;
		break;
	case KeyboardButton::Z:
		rotationSpeed = 0.0f;
		break;
	case KeyboardButton::X:
		rotationSpeed = 0.0f;
		break;
	}
}

void ShapeDrawingDemo::HandleScrollEvent(float scroll_amount)
{
	Demo::HandleScrollEvent(scroll_amount);
}

void ShapeDrawingDemo::HandleFocusEvent(bool focused)
{
	Demo::HandleFocusEvent(focused);
}
