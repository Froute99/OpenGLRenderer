/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *
 *	File_name: PhongShadingDemo.cpp
 *
 *	Simple Shape Drawing Demo
 *
 *	Nov.25 2019
 *******************************************************/

#include <glew.h>
#include <glfw3.h>
#include "TestStage.h"
#include "GameObject.h"
#include <Graphics/Draw.h>
#include <Graphics/PATH.h>
#include <Graphics/Screenshot.h>
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

#include <Graphics/Image.h>
#include "stb_image.h"
#include "stb_image_write.h"

#include "Skybox.h"

void TestStage::Initialize()
{
	shader.LoadShaderFrom(PATH::shape_vert, PATH::shape_frag);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	cube = GameObject::CreateCube({ 0, 0, 3.f }, { 0, 0, 0 }, 1);
	objectColor = { 1.0f, 0.5f, 0.31f };

	lightPos = { 0.0f, 2.45f, 4.85f };
	lightColor = { 1.0f, 1.0f, 1.0f };

	// uniform variable location
	uniformModelLocation = glGetUniformLocation(shader.GetHandleToShader(), "model");
	uniformViewLocation = glGetUniformLocation(shader.GetHandleToShader(), "view");
	uniformProjectionLocation = glGetUniformLocation(shader.GetHandleToShader(), "projection");

	uniformObjectColorLocation = glGetUniformLocation(shader.GetHandleToShader(), "objectColor");
	uniformLightPosLocation = glGetUniformLocation(shader.GetHandleToShader(), "lightPos");
	uniformLightColorLocation = glGetUniformLocation(shader.GetHandleToShader(), "lightColor");

}

void TestStage::Update(float /*dt*/)
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
	// Cube
	// ==================================

	Shader::UseShader(shader);
	const mat4<float>& Model = cube->GetModelToWorld();
	const mat4<float>& View = camera.BuildViewMatrix();
	const mat4<float>& Projection = view.BuildProjectionMatrix();
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, &Model.elements[0][0]);
	glUniformMatrix4fv(uniformViewLocation, 1, GL_FALSE, &View.elements[0][0]);
	glUniformMatrix4fv(uniformProjectionLocation, 1, GL_FALSE, &Projection.elements[0][0]);

	glUniform3fv(uniformObjectColorLocation, 1, &objectColor.x);
	glUniform3fv(uniformLightPosLocation, 1, &lightPos.x);
	glUniform3fv(uniformLightColorLocation, 1, &lightColor.x);

	cube->Draw();

	Draw::FinishDrawing();

	ImguiHelper();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TestStage::ResetCamera()
{
	// camera.ResetUp();
}

void TestStage::HandleResizeEvent(const int& new_width, const int& new_height)
{
	Demo::HandleResizeEvent(new_width, new_height);
}

void TestStage::HandleKeyPress(KeyboardButton button)
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

void TestStage::HandleKeyRelease(KeyboardButton button)
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

void TestStage::HandleScrollEvent(float scroll_amount)
{
	Demo::HandleScrollEvent(scroll_amount);
}

void TestStage::HandleFocusEvent(bool focused)
{
	Demo::HandleFocusEvent(focused);
}

void TestStage::ImguiHelper()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	{
		ImGui::Begin("Global");
		ImGui::SetWindowCollapsed(false);

		ImGui::Text("Phong Shading Model Demo."); // Display some text (you can use a format strings too)
		ImGui::NewLine();

		// toggle object window
		ImGui::Checkbox("Show Object Window", &showObjectWindow);

		// toggle light window
		ImGui::Checkbox("Show Light Window", &showLightWindow);

		ImGui::End();
	}
	// Object window
	{
		ImGui::Begin("Objects");
		ImGui::SetWindowCollapsed(false);
	}
	//ImGui::NewFrame();
	{
		ImGui::Begin("Lights");
		ImGui::SetWindowCollapsed(false);

		ImGui::ColorEdit3("Directional Light", (float*)&lightColor); // Edit 3 floats representing a color

		ImGui::NewLine();
		ImGui::Text("  Light Position");

		ImVec2 p = ImGui::GetCursorScreenPos();
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2);
		ImGui::BeginChild("X", { 100, 100 }, true);
		ImGui::Text("X");
		ImGui::DragFloat("", &lightPos.x, 0.05f);
		ImGui::EndChild();

		ImGui::SetNextWindowPos({ p.x + 150, p.y }, 0, ImVec2(0.4f, 0.0f));
		ImGui::BeginChild("Y", { 100, 100 }, true);
		ImGui::Text("Y");
		ImGui::DragFloat("", &lightPos.y, 0.05f);
		ImGui::EndChild();

		ImGui::SetNextWindowPos({ p.x + 250, p.y }, 0, ImVec2(0.3f, 0.0f));
		ImGui::BeginChild("Z", { 100, 100 }, true);
		ImGui::Text("Z");
		ImGui::DragFloat("", &lightPos.z, 0.05f);
		ImGui::EndChild();

		ImGui::PopStyleVar();
	}

	// Rendering
	ImGui::Render();
}
