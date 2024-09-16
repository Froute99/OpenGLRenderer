/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	
 *	File_name: TransformParentDemo.cpp
 *	
 *	Transform Set Parent Test
 *	
 *	Dec.07 2019
 *******************************************************/

#include "TransformParentDemo.hpp"
#include <Graphics/Draw.hpp>
#include <Graphics/PATH.hpp>
#include <Math/Angle.hpp>
#include <iostream>

namespace
{
	//Image From: http://www.pngall.com/sword-png/download/1376
	const std::filesystem::path& sword1_png = "../assets/sword1.png";

	const std::filesystem::path& slash1_png = "../assets/slash1.png";
	const std::filesystem::path& slash2_png = "../assets/slash2.png";
}

void TransformParentDemo::Initialize()
{
	shader.LoadShaderFrom(PATH::texture_vert, PATH::texture_frag);

	//const Mesh& rectangle = MESH::create_rectangle({ 0.0f }, { 100.0f });

	//sword1.shader = shader;
	//sword1.vertices.InitializeWithMeshAndLayout(rectangle, layout);
	//sword1.texture.LoadFromPath(sword1_png);
	//sword1Transform.SetScale(2.0f);
	//rotation = ANGLE::pi / 4.0f;

	//slash1.shader = shader;
	//slash1.vertices.InitializeWithMeshAndLayout(rectangle, layout);
	//slash1.texture.LoadFromPath(slash1_png);
	//slash1Transform.SetParent(&sword1Transform);
	//slash1Transform.SetScale(0.2f);
	//slash1Transform.SetTranslation({ 27.0f, -25.0f });

	//slash2.shader = shader;
	//slash2.vertices.InitializeWithMeshAndLayout(rectangle, layout);
	//slash2.texture.LoadFromPath(slash2_png);
	//slash2Transform.SetParent(&sword1Transform);
	//slash2Transform.SetScale(1.5f);
	//slash2Transform.SetRotation(ANGLE::pi / 4.0f);
	//slash2Transform.SetTranslation({ -10.0f, 20.0f });
	
	//view.SetViewSize(width, height);
	//cameraToNDC = view.GetCameraToNDCTransform() * camera.WorldToCamera();
	std::cout << "\t====================================\n";
	std::cout << "\tPress Q, E to rotate the sword\n";
	std::cout << "\tClick left mouse button to slash\n";
	std::cout << "\t====================================\n";
}

void TransformParentDemo::Update(float dt)
{
	std::cout << "\r" << dt;
	if (!isFocused)
	{
		Draw::StartDrawing();
		return;
	}
	slashRotation += dt;
	Draw::StartDrawing();

	//rotation += rotationSpeed * dt;
	//sword1Transform.SetRotation(rotation);
	//sword1Transform.SetTranslation(mousePosition);
	//sword1.ndc = cameraToNDC * sword1Transform.GetModelToWorld();
	//Draw::draw(sword1);

	//slash1Transform.SetRotation(slashRotation * 5.0f);
	//slash1.ndc = cameraToNDC * slash1Transform.GetModelToWorld();
	//Draw::draw(slash1);

	//if (isClicked)
	//{
	//	slash2.ndc = cameraToNDC * slash2Transform.GetModelToWorld();
	//	Draw::draw(slash2);
	//}
	
	Draw::FinishDrawing();	
}

void TransformParentDemo::ResetCamera()
{
	//camera.ResetUp();
}

void TransformParentDemo::HandleKeyPress(KeyboardButton button)
{
	switch (button)
	{
	case KeyboardButton::Q:
		rotationSpeed = 1.0f;
		break;
	case KeyboardButton::E:
		rotationSpeed = -1.0f;
		break;
	}
}

void TransformParentDemo::HandleResizeEvent(const int& new_width, const int& new_height)
{
	Demo::HandleResizeEvent(new_width, new_height);
}

void TransformParentDemo::HandleMousePositionEvent(float xpos, float ypos)
{
	mousePosition.x = xpos - float(width) / 2.0f;
	mousePosition.y = -ypos + float(height) / 2.0f;
}

void TransformParentDemo::HandleMouseEvent(MouseButton button)
{
	switch (button)
	{
	case MouseButton::LEFT_PRESS:
		isClicked = true;
		break;
	case MouseButton::LEFT_RELEASE:
		isClicked = false;
		break;
	}
}

void TransformParentDemo::HandleFocusEvent(bool focused)
{
	Demo::HandleFocusEvent(focused);
}
