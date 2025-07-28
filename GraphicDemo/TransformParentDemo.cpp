/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	
 *	File_name: TransformParentDemo.cpp
 *	
 *	Transform Set Parent Test
 *	
 *	Dec.07 2019
 *******************************************************/

#include "TransformParentDemo.h"
#include <Graphics/Draw.h>
#include "GameObject.h"
#include <iostream>
#include <glew.h>

void TransformParentDemo::Initialize()
{
	shader.LoadShaderFrom("../assets/shaders/texture.vert", "../assets/shaders/texture.frag");

	camera.SetEyePosition({ 0.f, 5.f, 30.f });

	// SUN
	sun = GameObject::CreateSphere({ 0 });
	sun->Move({ 0.f, 0.f, 0.f });
	sun->Rotate({ 3.141592f, 0.f, 0.f });
	sun->Scale(10.f);

	sunAlbedo = new Texture();
	sunAlbedo->LoadFromPath("../assets/Models/Image_24.png", false);

	// EARTH
	earth = GameObject::CreateSphere({ 0 });
	earth->SetParent(sun);
	earth->Move({ 5.f, 0.f, 0.f });
	earth->Rotate({ 3.141592f, 0.f, 0.f });
	earth->Scale(0.1f);

	earthAlbedo = new Texture();
	earthAlbedo->LoadFromPath("../assets/Models/Image_14.png", false);

	// MOON
	moon = GameObject::CreateSphere({ 0 });
	moon->SetParent(earth);
	moon->Move({ 7.f, 0.f, 0.f });
	//moon->Rotate({ 3.141592f, 0.f, 0.f });
	moon->Scale(0.7f);
	moonAlbedo = new Texture();
	moonAlbedo->LoadFromPath("../assets/Models/moon.png");

	matricesBlock.BindTo(shader.GetHandleToShader(), "Matrices");

	lightPosition = sun->GetTransform()->GetTranslation();
	lightColor = { 10.f };

	//view.SetViewSize(width, height);
	//cameraToNDC = view.GetCameraToNDCTransform() * camera.WorldToCamera();
}

void TransformParentDemo::Update(float dt)
{
	//if (!isFocused)
	Demo::Update(dt);
	Draw::StartDrawing();

	sun->Rotate({ 0.f, 0.001f, 0.f });
	earth->Rotate({ 0.f, 0.01f, 0.f });
	//moon->Rotate({ 0.f, 0.01f, 0.f });

	const mat4<float>& View = camera.BuildViewMatrix();
	const mat4<float>& Projection = view.BuildProjectionMatrix();

	matricesBlock.WriteData(0, 64, &View);
	matricesBlock.WriteData(64, 64, &Projection);

	Shader::UseShader(shader);
	mat4<float> Model = sun->GetModelToWorld();

	shader.SendUniformVariable("model", Model);
	shader.SendUniformVariable("lightPosition", lightPosition);
	shader.SendUniformVariable("lightColor", lightColor);
	shader.BindTexture("textureDiffuse", 0, sunAlbedo->GetTexturehandle());
	sun->Draw();

	//Model = sun->GetModelToWorld() * earth->GetModelToWorld();
	Model = earth->GetModelToWorld();
	shader.SendUniformVariable("model", Model);
	shader.SendUniformVariable("lightPosition", lightPosition);
	shader.SendUniformVariable("lightColor", lightColor);
	shader.BindTexture("textureDiffuse", 0, earthAlbedo->GetTexturehandle());
	earth->Draw();

	//Model = sun->GetModelToWorld() * earth->GetModelToWorld() * moon->GetModelToWorld();
	Model = moon->GetModelToWorld();
	shader.SendUniformVariable("model", Model);
	shader.SendUniformVariable("lightPosition", lightPosition);
	shader.SendUniformVariable("lightColor", lightColor);
	shader.BindTexture("textureDiffuse", 0, moonAlbedo->GetTexturehandle());
	moon->Draw();

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
		case KeyboardButton::W:
			cameraMovement.z = 0.1f;
			break;
		case KeyboardButton::A:
			cameraMovement.x = -0.1f;
			break;
		case KeyboardButton::S:
			cameraMovement.z = -0.1f;
			break;
		case KeyboardButton::D:
			cameraMovement.x = 0.1f;
			break;
		case KeyboardButton::Q:
			cameraMovement.y = -0.1f;
			break;
		case KeyboardButton::E:
			cameraMovement.y = 0.1f;
			break;
	}
}

void TransformParentDemo::HandleKeyRelease(KeyboardButton button)
{
	switch (button)
	{
		case KeyboardButton::W:
			cameraMovement.z = 0.0f;
			break;
		case KeyboardButton::A:
			cameraMovement.x = 0.0f;
			break;
		case KeyboardButton::S:
			cameraMovement.z = 0.0f;
			break;
		case KeyboardButton::D:
			cameraMovement.x = 0.0f;
			break;
		case KeyboardButton::Q:
			cameraMovement.y = 0.0f;
			break;
		case KeyboardButton::E:
			cameraMovement.y = 0.0f;
			break;
	}
}

void TransformParentDemo::HandleResizeEvent(const int new_width, const int new_height)
{
	Demo::HandleResizeEvent(new_width, new_height);
}

void TransformParentDemo::HandleMousePositionEvent(float xpos, float ypos)
{

}

void TransformParentDemo::HandleMouseEvent(MouseButton button)
{

}

void TransformParentDemo::HandleFocusEvent(bool focused)
{
	Demo::HandleFocusEvent(focused);
}
