/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *
 *	File_name: TextureDrawingDemo.cpp
 *
 *	Simple Texture Drawing Demo
 *
 *	Nov.29 2019
 *******************************************************/

#include <iostream>
#include "glfw3.h"
#include "TextureDrawingDemo.hpp"
#include "PATH.hpp"

void TextureDrawingDemo::Initialize()
{
	shader.LoadShaderFrom(PATH::texture_vert, PATH::texture_frag);

	//const Mesh& rectangle = MESH::create_rectangle({ 0.0f }, { 1.0f }, { 0.0f });

	//dragon.shader = shader;
	//dragon.vertices.InitializeWithMeshAndLayout(rectangle, layout);
	//dragon.texture.LoadFromPath(dragon_png);
	//dragonTransform.SetScale(500.0f);

	//blueFire.shader.LoadShaderFrom(PATH::animation_vert, PATH::animation_frag);
	//blueFire.vertices.InitializeWithMeshAndLayout(rectangle, layout);
	//blueFire.texture.LoadFromPath(blue_fire_sprite_sheet);
	//blueFireTransform.SetScale(130.0f);

	//blueFireAnimation.Initialize({ 10, 6, 25.0f }, blueFire.shader);

	font.LoadFromFile(PATH::bitmapfont_fnt);
	text.SetFont(font);
	text.SetString(L"Blue Fire Dragon");
	textTransform.SetTranslation({ -500.0f, 300.0f });

	view.SetViewSize(width, height);
	cameraToNDC = view.GetCameraToNDCTransform() * camera.WorldToCamera();
	timePassed = 0.0f;
	std::cout << "\t====================================\n";
	std::cout << "\tPress R key to animate from start\n";
	std::cout << "\t====================================\n";
}

void TextureDrawingDemo::Update(float dt)
{
	std::cout << "\r" << dt;
	if (!isFocused)
	{
		Draw::StartDrawing();
		return;
	}
	timePassed += dt;
	Draw::StartDrawing();

	const mat3<float> ndc = cameraToNDC * textTransform.GetModelToWorld();
	Draw::DrawText(shader, ndc, text);

	//if (timePassed >= 1.0f)
	//{
	//	dragon.ndc = cameraToNDC * dragonTransform.GetModelToWorld();
	//	Draw::draw(dragon);
	//}
	//if (timePassed >= 1.4f)
	//{
	//	blueFireTransform.SetTranslation({ -100.0f, 120.0f });
	//	blueFire.ndc = cameraToNDC * blueFireTransform.GetModelToWorld();
	//	blueFireAnimation.Animate(dt);
	//	Draw::draw(blueFire);
	//}
	//if (timePassed >= 0.2f)
	//{
	//	blueFireTransform.SetTranslation({ fireDistance, 0.0f });
	//	blueFire.ndc = cameraToNDC * blueFireTransform.GetModelToWorld();
	//	blueFireAnimation.Animate(dt);
	//	Draw::draw(blueFire);
	//}
	//if (timePassed >= 0.4f)
	//{
	//	blueFireTransform.SetTranslation({ -fireDistance, 0.0f });
	//	blueFire.ndc = cameraToNDC * blueFireTransform.GetModelToWorld();
	//	blueFireAnimation.Animate(dt);
	//	Draw::draw(blueFire);
	//}
	//if (timePassed >= 0.6f)
	//{
	//	blueFireTransform.SetTranslation({ 0.0f, fireDistance });
	//	blueFire.ndc = cameraToNDC * blueFireTransform.GetModelToWorld();
	//	blueFireAnimation.Animate(dt);
	//	Draw::draw(blueFire);
	//}
	//if (timePassed >= 0.8f)
	//{
	//	blueFireTransform.SetTranslation({ 0.0f, -fireDistance });
	//	blueFire.ndc = cameraToNDC * blueFireTransform.GetModelToWorld();
	//	blueFireAnimation.Animate(dt);
	//	Draw::draw(blueFire);
	//}
	
	Draw::FinishDrawing();
}

void TextureDrawingDemo::ResetCamera()
{
	camera.ResetUp();
}

void TextureDrawingDemo::HandleKeyPress(KeyboardButton button)
{
	if (button == KeyboardButton::R)
	{
		timePassed = 0.0f;
	}
}

void TextureDrawingDemo::HandleResizeEvent(const int& new_width, const int& new_height)
{
	Demo::HandleResizeEvent(new_width, new_height);
}

void TextureDrawingDemo::HandleFocusEvent(bool focused)
{
	Demo::HandleFocusEvent(focused);
}

void TextureDrawingDemo::HandleScrollEvent(float scroll_amount)
{
	Demo::HandleScrollEvent(scroll_amount);
}
