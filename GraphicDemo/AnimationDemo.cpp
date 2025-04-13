/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	
 *	File_name: AnimationDemo.cpp
 *	
 *	Animation Displaying
 *	
 *	Dec.03 2019
 *******************************************************/

#include "AnimationDemo.h"
#include <Graphics/Draw.h>
#include <Graphics/PATH.h>
#include <iostream>

const std::filesystem::path& fire_png = "../assets/fire.png";
const std::filesystem::path& blue_fire_png = "../assets/blue_fire.png";

void AnimationDemo::Initialize()
{
	shader.LoadShaderFrom(PATH::animation_vert, PATH::animation_frag);

	//const Mesh& rectangle = MESH::create_rectangle({ 0.0f }, { 1.0f }, { 0.0f });

	//fire.shader = shader;
	//fire.vertices.InitializeWithMeshAndLayout(rectangle, layout);
	//fire.texture.LoadFromPath(fire_png);
	//fireTransform.SetScale(100.0f);
	//fireTransform.SetTranslation({ 50.0f, 0.0f });

	//blueFire.shader = shader;
	//blueFire.vertices.InitializeWithMeshAndLayout(rectangle, layout);
	//blueFire.texture.LoadFromPath(blue_fire_png);
	//blueFireTransform.SetScale(100.0f);
	//blueFireTransform.SetTranslation({ -50.0f, 0.0f });

	//fireAnimation.Initialize({ 4, 4, 20.0f }, shader);
	//blueFireAnimation.Initialize({ 10, 6, 30.0f }, shader);

	//view.SetViewSize(width, height);
	//cameraToNDC = view.GetCameraToNDCTransform() * camera.WorldToCamera();
}

void AnimationDemo::Update(float dt)
{
	std::cout << "\r" << dt;
	if (!isFocused)
	{
		Draw::StartDrawing();
		return;
	}
	Draw::StartDrawing();

	//fire.ndc = cameraToNDC * fireTransform.GetModelToWorld();
	//blueFire.ndc = cameraToNDC * blueFireTransform.GetModelToWorld();

	//fireAnimation.Animate(dt);
	//Draw::draw(fire);
	//blueFireAnimation.Animate(dt);
	//Draw::draw(blueFire);
	
	Draw::FinishDrawing();
}

void AnimationDemo::ResetCamera()
{
	//camera.ResetUp();
}

void AnimationDemo::HandleResizeEvent(const int& new_width, const int& new_height)
{
	Demo::HandleResizeEvent(new_width, new_height);
}

void AnimationDemo::HandleFocusEvent(bool focused)
{
	Demo::HandleFocusEvent(focused);
}
