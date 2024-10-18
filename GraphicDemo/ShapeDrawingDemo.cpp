/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *
 *	File_name: ShapeDrawingDemo.cpp
 *
 *	Simple Shape Drawing Demo
 *
 *	Nov.25 2019
 *******************************************************/

#include <glew.h>
#include <glfw3.h>
#include "ShapeDrawingDemo.hpp"
#include "GameObject.h"
#include <Graphics/Draw.hpp>
#include <Graphics/PATH.hpp>
#include <Graphics/Screenshot.hpp>
#include <Graphics/Mesh3D.h>

#include <Math/Angle.hpp>
#include <Math/vec3.hpp>

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Graphics/Image.hpp>
#include "stb_image.h"
#include "stb_image_write.h"

#include "Skybox.h"

void ShapeDrawingDemo::Initialize()
{
	shader.LoadShaderFrom(PATH::shape_vert, PATH::shape_frag);
	skyboxShader.LoadShaderFrom(PATH::skyboxVS, PATH::skyboxFS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	const std::string& filename = "../assets/backpack/backpack.obj";
	backpack = GameObject::LoadMeshFromFile(filename);

	vec3<float> cubePos(0.0f, 0.0f, 5.0f);
	cubeTranslation = cubePos;
	backpack->Move(cubePos);
	objectColor = { 1.0f, 0.5f, 0.31f };

	lightPos = { 0.7f, 0.4f, 0.8f };
	lightColor = { 1.0f, 1.0f, 1.0f };

	// uniform variable location
	uniformModelLocation = glGetUniformLocation(shader.GetHandleToShader(), "model");
	uniformViewLocation = glGetUniformLocation(shader.GetHandleToShader(), "view");
	uniformProjectionLocation = glGetUniformLocation(shader.GetHandleToShader(), "projection");

	uniformObjectColorLocation = glGetUniformLocation(shader.GetHandleToShader(), "objectColor");
	uniformLightPosLocation = glGetUniformLocation(shader.GetHandleToShader(), "lightPos");
	uniformLightColorLocation = glGetUniformLocation(shader.GetHandleToShader(), "lightColor");

	uniformLightCubeModel = glGetUniformLocation(skyboxShader.GetHandleToShader(), "model");
	uniformLightCubeView = glGetUniformLocation(skyboxShader.GetHandleToShader(), "view");
	uniformLightCubeProjection = glGetUniformLocation(skyboxShader.GetHandleToShader(), "projection");

	// ==================================
	// skybox
	// ==================================

	skybox = new Skybox();
}

void ShapeDrawingDemo::Update(float dt)
{
	// if (!isFocused)
	//{
	//	Draw::StartDrawing();
	//	return;
	// }

	// std::cout << "\r" << dt;

	 camera.MoveX(moveSpeed.x);
	 camera.MoveY(moveSpeed.y);
	 camera.MoveZ(moveSpeed.z);

	Draw::StartDrawing();

	// ==================================
	// Simple Cube
	// ==================================

	Shader::UseShader(shader);
	backpack->Rotate({ 0.f, ANGLE::DegreeToRadian(35.f * angle * dt), 0.f });
	const mat4<float>& Model = backpack->GetModelToWorld();
	const mat4<float>& View = camera.BuildViewMatrix();
	const mat4<float>& Projection = view.BuildProjectionMatrix();
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, &Model.elements[0][0]);
	glUniformMatrix4fv(uniformViewLocation, 1, GL_FALSE, &View.elements[0][0]);
	glUniformMatrix4fv(uniformProjectionLocation, 1, GL_FALSE, &Projection.elements[0][0]);

	glUniform3fv(uniformObjectColorLocation, 1, &objectColor.x);
	glUniform3fv(uniformLightPosLocation, 1, &lightPos.x);
	glUniform3fv(uniformLightColorLocation, 1, &lightColor.x);

	glUniform1i(glGetUniformLocation(shader.GetHandleToShader(), "textureDiffuse1"), 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, backpack->GetTextureHandle(0));

	backpack->Draw();

	// ==================================
	// Skybox
	// ==================================
	Shader::UseShader(skyboxShader);
	//const mat4<float>& LightCubeModel = cube->GetModelToWorld();
	const mat4<float>& LightCubeModel = skybox->GetModelToWorld();
	const mat4<float>& skyboxView = Matrix4::CutOffTranslation(View); // remove translation from origin view matrix
	glUniformMatrix4fv(uniformLightCubeModel, 1, GL_FALSE, &LightCubeModel.elements[0][0]);
	glUniformMatrix4fv(uniformLightCubeView, 1, GL_FALSE, &skyboxView.elements[0][0]);
	glUniformMatrix4fv(uniformLightCubeProjection, 1, GL_FALSE, &Projection.elements[0][0]);

	skybox->Draw();

	Draw::FinishDrawing();

	ImguiHelper();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ShapeDrawingDemo::ResetCamera()
{
	// camera.ResetUp();
}

void ShapeDrawingDemo::HandleResizeEvent(const int& new_width, const int& new_height)
{
	Demo::HandleResizeEvent(new_width, new_height);
}

void ShapeDrawingDemo::HandleKeyPress(KeyboardButton button)
{
	switch (button)
	{
		case KeyboardButton::W:
			moveSpeed.z = 0.1f;
			break;
		case KeyboardButton::A:
			moveSpeed.x = -0.1f;
			break;
		case KeyboardButton::S:
			moveSpeed.z = -0.1f;
			break;
		case KeyboardButton::D:
			moveSpeed.x = 0.1f;
			break;
		case KeyboardButton::Q:
			moveSpeed.y = 0.1f;
			break;
		case KeyboardButton::E:
			moveSpeed.y = -0.1f;
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
		case KeyboardButton::W:
			moveSpeed.z = 0.0f;
			break;
		case KeyboardButton::A:
			moveSpeed.x = 0.0f;
			break;
		case KeyboardButton::S:
			moveSpeed.z = 0.0f;
			break;
		case KeyboardButton::D:
			moveSpeed.x = 0.0f;
			break;
		case KeyboardButton::Q:
			moveSpeed.y = 0.0f;
			break;
		case KeyboardButton::E:
			moveSpeed.y = 0.0f;
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

void ShapeDrawingDemo::ImguiHelper()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	{
		ImGui::Begin("Global"); // Create a window called "Hello, world!" and append into it.
		ImGui::SetWindowCollapsed(false);

		ImGui::Text("Phong Shading Model Demo."); // Display some text (you can use a format strings too)

		ImGui::ColorEdit3("Light color", (float*)&lightColor); // Edit 3 floats representing a color
		ImGui::ColorEdit3("Cube color", (float*)&objectColor); // Edit 3 floats representing a

		ImGui::NewLine();
		ImGui::DragFloat3("Drag Float3", &cubeTranslation.x, 0.1f);
		//cube->GetTransform()->SetTranslation(cubeTranslation);

		ImGui::End();
	}

	// Rendering
	ImGui::Render();
}
