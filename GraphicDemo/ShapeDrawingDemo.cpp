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
#include "GameObject.h"

void ShapeDrawingDemo::Initialize()
{
	shader.LoadShaderFrom(PATH::shape_vert, PATH::shape_frag);
	layout = { VerticesDescription::Type::Point, VerticesDescription::Type::Color };

	const Color4f color{ .8f, .8f, .0f, 1.0f };

	vec2<float> size{ 50.0f };
	const Mesh rectangleMesh = MESH::create_rectangle({ 0.0f }, size, color);
	rectangle = new GameObject(rectangleMesh, layout);
	rectangle->SetShader(&shader);
	//rectangleVertices.InitializeWithMeshAndLayout(rectangle, layout);

	const Mesh lineMesh = MESH::create_line({ 100.0f, 0.0f }, { 50.0f, 50.0f }, { -50.0f, -50.0f }, color);
	line = new GameObject(lineMesh, layout);
	line->SetShader(&shader);
	//lineVertices.InitializeWithMeshAndLayout(line, layout);

	const Mesh quadMesh = MESH::create_quad({ 0.0f, 100.0f }, { -20.f, 25.f }, { -50.f, -25.f }, { 50.f, -25.f }, { 20.f, 25.f }, color);
	quad = new GameObject(quadMesh, layout);
	quad->SetShader(&shader);
	//quadVertices.InitializeWithMeshAndLayout(quad, layout);

	const Mesh triangleMesh = MESH::create_triangle({ 100.0f }, { 0.0f, 50.0f }, { -30.f, 0.0f }, { 30.f, 0.0f }, color);
	triangle = new GameObject(triangleMesh, layout);
	triangle->SetShader(&shader);
	//triangleVertices.InitializeWithMeshAndLayout(triangle, layout);

	const Mesh circleMesh = MESH::create_ellipse({ 200.0f, 25.0f }, size, 30, color);
	circle = new GameObject(circleMesh, layout);
	circle->SetShader(&shader);
	//circleVertices.InitializeWithMeshAndLayout(circle, layout);

	const Mesh ellipseMesh = MESH::create_ellipse({ -200.0f, 25.0f }, { 50.0f, 30.f }, 30, color);
	ellipse = new GameObject(ellipseMesh, layout);
	ellipse->SetShader(&shader);
	//ellipseVertices.InitializeWithMeshAndLayout(ellipse, layout);

	view.SetViewSize(width, height);
	std::cout << "\t====================================\n";
	std::cout << "\tPress Arrow keys to move the camera\n";
	std::cout << "\tPress Z, X keys to rotate the camera\n";
	std::cout << "\tPress \"Enter\" key to screenshot\n";
	std::cout << "\t====================================\n";

	const mat3<float> cameraToNDC = view.GetCameraToNDCTransform() * camera.WorldToCamera();

	rectangle->SetNDC(cameraToNDC);
	line->SetNDC(cameraToNDC);
	quad->SetNDC(cameraToNDC);
	triangle->SetNDC(cameraToNDC);
	circle->SetNDC(cameraToNDC);
	ellipse->SetNDC(cameraToNDC);

	//ndc = cameraToNDC * rectangle->GetTransform()->GetModelToWorld();
	//lineNDC = cameraToNDC * line->GetTransform()->GetModelToWorld();
	//quadNDC = cameraToNDC * quad->GetTransform()->GetModelToWorld();
	//ellipseNDC = cameraToNDC * ellipse->GetTransform()->GetModelToWorld();
	//triangleNDC = cameraToNDC * triangle->GetTransform()->GetModelToWorld();
	//circleNDC = cameraToNDC * circle->GetTransform()->GetModelToWorld();
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

	Draw::DrawShape(*rectangle->GetMaterial());
	Draw::DrawShape(*line->GetMaterial());
	Draw::DrawShape(*quad->GetMaterial());
	Draw::DrawShape(*triangle->GetMaterial());
	Draw::DrawShape(*circle->GetMaterial());
	Draw::DrawShape(*ellipse->GetMaterial());

	//Draw::DrawGameObject(DrawType::Shape, rectangle->GetMaterial());
	//Draw::DrawGameObject(DrawType::Shape, line->GetMaterial());
	//Draw::DrawGameObject(DrawType::Shape, quad->GetMaterial());
	//Draw::DrawGameObject(DrawType::Shape, triangle->GetMaterial());
	//Draw::DrawGameObject(DrawType::Shape, circle->GetMaterial());
	//Draw::DrawGameObject(DrawType::Shape, ellipse->GetMaterial());

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
