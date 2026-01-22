/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	
 *	File_name: TransformHierarchyDemo.cpp
 *	
 *	Transform Set Parent Test
 *	
 *	Dec.07 2019
 *******************************************************/

#include "TransformHierarchyDemo.h"
#include <Graphics/Draw.h>
#include "SceneObject.h"
#include <iostream>
#include <glew.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

void TransformHierarchyDemo::Initialize()
{
	shader.CanLoadShader("../assets/shaders/texture.vert", "../assets/shaders/texture.frag");

	camera.SetEyePosition({ 0.f, 5.f, 30.f });

	sunPosition = { 0.f, 0.f, 0.f };
	earthPosition = { 5.f, 0.f, 0.f };
	moonPosition = { 7.f, 0.f, 0.f };

	// SUN
	sun = SceneObject::CreateSphere({ 0 });
	sun->Move(sunPosition);
	sun->Rotate({ 3.141592f, 0.f, 0.f });
	sun->Scale(10.f);

	sunAlbedo = new Texture();
	sunAlbedo->LoadFromPath("../assets/Models/Image_24.png", false);

	// EARTH
	earth = SceneObject::CreateSphere({ 0 });
	earth->SetParent(sun);
	earth->Move(earthPosition);
	earth->Rotate({ 3.141592f, 0.f, 0.f });
	earth->Scale(0.1f);

	earthAlbedo = new Texture();
	earthAlbedo->LoadFromPath("../assets/Models/Image_14.png", false);

	// MOON
	moon = SceneObject::CreateSphere({ 0 });
	moon->SetParent(earth);
	moon->Move(moonPosition);
	moon->Rotate({ 3.141592f, 0.f, 0.f });
	moon->Scale(0.7f);
	moonAlbedo = new Texture();
	moonAlbedo->LoadFromPath("../assets/Models/moon.png");

	//matricesBlock.BindTo(shader.GetHandleToShader(), "Matrices");

	lightPosition = sun->GetTransform()->GetTranslation();
	lightColor = { 10.f };

	//view.SetViewSize(width, height);
	//cameraToNDC = view.GetCameraToNDCTransform() * camera.WorldToCamera();
}

void TransformHierarchyDemo::Update(float /*dt*/)
{
	//if (!isFocused)
	if (anyChange)
	{
		sun->GetTransform()->SetTranslation(sunPosition);
		earth->GetTransform()->SetTranslation(earthPosition);
		moon->GetTransform()->SetTranslation(moonPosition);
		anyChange = false;
	}

	sun->Rotate({ 0.f, 0.001f, 0.f });
	earth->Rotate({ 0.f, 0.01f, 0.f });
	moon->Rotate({ 0.f, 0.01f, 0.f });

	const mat4<float>& View = camera.BuildViewMatrix();
	const mat4<float>& Projection = view.BuildProjectionMatrix();

	//matricesBlock.WriteData(0, 64, &View);
	//matricesBlock.WriteData(64, 64, &Projection);

	shader.Use();
	mat4<float> Model = sun->GetModelToWorld();

	shader.SendUniformVariable("model", Model);
	shader.SendUniformVariable("lightPosition", sun->GetTransform()->GetTranslation());
	shader.SendUniformVariable("lightColor", lightColor);
	shader.BindTexture("textureDiffuse", 0, sunAlbedo->GetTexturehandle());
	sun->Draw();

	Model = earth->GetModelToWorld();
	shader.SendUniformVariable("model", Model);
	shader.SendUniformVariable("lightPosition", lightPosition);
	shader.SendUniformVariable("lightColor", lightColor);
	shader.BindTexture("textureDiffuse", 0, earthAlbedo->GetTexturehandle());
	earth->Draw();

	Model = moon->GetModelToWorld();
	shader.SendUniformVariable("model", Model);
	shader.SendUniformVariable("lightPosition", lightPosition);
	shader.SendUniformVariable("lightColor", lightColor);
	shader.BindTexture("textureDiffuse", 0, moonAlbedo->GetTexturehandle());
	moon->Draw();
}

void TransformHierarchyDemo::ResetCamera()
{
	//camera.ResetUp();
}

void TransformHierarchyDemo::HandleKeyPress(KeyboardButton button)
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

void TransformHierarchyDemo::HandleKeyRelease(KeyboardButton button)
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

void TransformHierarchyDemo::HandleResizeEvent(const int new_width, const int new_height)
{
	Demo::HandleResizeEvent(new_width, new_height);
}

void TransformHierarchyDemo::HandleMousePositionEvent(float xpos, float ypos)
{

}

void TransformHierarchyDemo::HandleMouseEvent(MouseButton button)
{

}

void TransformHierarchyDemo::HandleFocusEvent(bool focused)
{
	Demo::HandleFocusEvent(focused);
}

void TransformHierarchyDemo::DrawGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();

	// Global
	ImGui::Begin("Global");
	ImGui::SetWindowCollapsed(false);

	ImGui::Text("Transform Hierarchy Demo");
	ImGui::Text("Solar System");
	ImGui::NewLine();
	ImGui::Text("Controls:");
	ImGui::Text("WASD - Move around");
	ImGui::Text("QE - Go up and down");
	ImGui::Text("Page up & down - Convert current showing demo stage");

	ImGui::End();

	// Object Property
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize;
	ImGui::Begin("PBR Properties", NULL, flags);
	ImGui::SetWindowCollapsed(false);
	ImGui::SetWindowSize({ 300, 150 });

	ImGui::NewLine();
	anyChange = anyChange || ImGui::DragFloat3("Sun", &sunPosition.x, 0.1f);
	anyChange = anyChange || ImGui::DragFloat3("Earth", &earthPosition.x, 0.1f);
	anyChange = anyChange || ImGui::DragFloat3("Moon", &moonPosition.x, 0.1f);

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
