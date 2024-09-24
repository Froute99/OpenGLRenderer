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

void ShapeDrawingDemo::Initialize()
{
	shader.LoadShaderFrom(PATH::shape_vert, PATH::shape_frag);
	lightCubeShader.LoadShaderFrom(PATH::lightCubeVS, PATH::lightCubeFS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	Assimp::Importer importer;

	//const std::string& filename = "../assets/sphere.fbx";	
	const std::string& filename = "../assets/backpack/backpack.obj";

	fbxCube = GameObject::LoadMeshFromFBX(filename);
	//std::cout << filename << " has loaded\n";
	//std::cout << fbxCube->GetMeshesCount() << " meshes\n";

	vec3<float> cubePos(0.0f, 0.0f, 5.0f);
	cubeTranslation = cubePos;
	fbxCube->Move(cubePos);
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

	uniformLightCubeModel = glGetUniformLocation(lightCubeShader.GetHandleToShader(), "model");
	uniformLightCubeView = glGetUniformLocation(lightCubeShader.GetHandleToShader(), "view");
	uniformLightCubeProjection = glGetUniformLocation(lightCubeShader.GetHandleToShader(), "projection");

	angle = 1.f;
}

void ShapeDrawingDemo::Update(float dt)
{
	// if (!isFocused)
	//{
	//	Draw::StartDrawing();
	//	return;
	// }

	// std::cout << "\r" << dt;

	// camera.Rotate(rotationSpeed);
	// camera.MoveRight(moveSpeed.x);
	// camera.MoveUp(moveSpeed.y);
	// camera.MoveFront(moveSpeed.y);

	ImguiHelper();

	Draw::StartDrawing();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//==================================
	// Simple Cube
	//==================================
	fbxCube->Rotate({ 0.f, ANGLE::DegreeToRadian(35.f * angle * dt), 0.f });
	const mat4<float>& Model = fbxCube->GetModelToWorld();
	const mat4<float>& View = camera.BuildViewMatrix();
	const mat4<float>& Projection = view.BuildProjectionMatrix();
	// column-major operations should be ordered like v2 = PVM * v1;

	Shader::UseShader(shader);
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, &Model.elements[0][0]);
	glUniformMatrix4fv(uniformViewLocation, 1, GL_FALSE, &View.elements[0][0]);
	glUniformMatrix4fv(uniformProjectionLocation, 1, GL_FALSE, &Projection.elements[0][0]);

	glUniform3fv(uniformObjectColorLocation, 1, &objectColor.x);
	glUniform3fv(uniformLightPosLocation, 1, &lightPos.x);
	glUniform3fv(uniformLightColorLocation, 1, &lightColor.x);

	glActiveTexture(GL_TEXTURE0);
	// now set the sampler to the correct texture unit
	glUniform1i(glGetUniformLocation(shader.GetHandleToShader(), "textureDiffuse1"), 0);
	// and finally bind the texture
	glBindTexture(GL_TEXTURE_2D, fbxCube->GetTextureHandle(0));

	fbxCube->Draw();
	glActiveTexture(GL_TEXTURE0);

	//==================================
	// Light Cube
	//==================================
	// const mat4<float>& LightCubeModel = lightCube->GetModelToWorld();
	// Shader::UseShader(lightCubeShader);
	// glUniformMatrix4fv(uniformLightCubeModel, 1, GL_FALSE, &LightCubeModel.elements[0][0]);
	// glUniformMatrix4fv(uniformLightCubeView, 1, GL_FALSE, &View.elements[0][0]);
	// glUniformMatrix4fv(uniformLightCubeProjection, 1, GL_FALSE, &Projection.elements[0][0]);

	// lightCube->Draw();

	Draw::FinishDrawing();
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

void ShapeDrawingDemo::ImguiHelper()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	{
		ImGui::Begin("Global"); // Create a window called "Hello, world!" and append into it.
		ImGui::SetWindowCollapsed(false);

		ImGui::Text("This is some useful text.");		   // Display some text (you can use a format strings too)

		ImGui::ColorEdit3("Light color", (float*)&lightColor); // Edit 3 floats representing a color
		ImGui::ColorEdit3("Cube color", (float*)&objectColor); // Edit 3 floats representing a 

		ImGui::NewLine();
		ImGui::DragFloat3("Drag Float3", &cubeTranslation.x, 0.1f);
		fbxCube->GetTransform()->SetTranslation(cubeTranslation);

		ImGui::End();
	}

	// Rendering
	ImGui::Render();
}
