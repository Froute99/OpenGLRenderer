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

#include "Skybox.h"

void TestStage::Initialize()
{
	//cube = GameObject::CreateCube({ 0, 0, -5 }, { 0, 0, 0 }, 1);
	//const std::string& filename = "../assets/Models/backpack.obj";
	//cube = GameObject::LoadMeshFromFile(filename);

	//cube->Rotate({ 0.f, -0.6f, 0.f });

	//objectColor = { 1.0f, 0.5f, 0.31f };

	//lightPos = { 3.0f, 0.00f, -5.00f };
	//lightColor = { 1.0f, 1.0f, 1.0f };



	mat4<float> projection = view.BuildProjectionMatrix();
	mat4<float> View = camera.BuildViewMatrix();
	bool succeed = envMap.CanLoad("../assets/newport_loft.hdr", projection);
	if (!succeed)
	{
		std::cout << "Failed\n";
	}
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
	// Equirectangular
	// ==================================

	//Shader::UseShader(shader);
	//const mat4<float>& Model = cube->GetModelToWorld();
	//const mat4<float>& View = camera.BuildViewMatrix();
	//const mat4<float>& Projection = view.BuildProjectionMatrix();

	//glm::mat4 View = glm::lookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

	//shader.SendUniformVariable("model", Model);
	//shader.SendUniformVariable("view", Matrix4::CutOffTranslation(View));
	//shader.SendUniformVariable("projection", Projection);

	//shader.SendUniformVariable("objectColor", objectColor);
	//shader.SendUniformVariable("lightPos", lightPos);
	//shader.SendUniformVariable("lightColor", lightColor);

	//cube->Draw();

	// ==================================
	// Skybox
	// ==================================

	envMap.Render(Matrix4::CutOffTranslation(camera.BuildViewMatrix()));

	Draw::FinishDrawing();

	//ImguiHelper();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

void TestStage::renderCube()
{
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,	// top-right
			1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,	// bottom-right
			1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,	// top-right
			-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,	// top-left
			// front face
			-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
			1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
			1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,	  // top-right
			1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,	  // top-right
			-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
			-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,	// top-right
			-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	// top-left
			-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// bottom-right
			-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,	// top-right
																// right face
			1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,		// top-left
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// bottom-right
			1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	// top-right
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// bottom-right
			1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,		// top-left
			1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// bottom-left
			// bottom face
			-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,	// top-left
			1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// bottom-left
			1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// bottom-left
			-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,	// bottom-right
			-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
			1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	  // bottom-right
			1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
			1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	  // bottom-right
			-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
			-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f	  // bottom-left
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
