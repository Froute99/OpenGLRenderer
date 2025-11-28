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
#include "PhongShadingDemo.h"
#include "SceneObject.h"
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

void PhongShadingDemo::Initialize()
{
	shader.LoadShaderFrom(PATH::texture_vert, PATH::texture_frag);
	skyboxShader.LoadShaderFrom(PATH::skyboxVS, PATH::skyboxFS);

	const std::string& filename = "../assets/Models/backpack.obj";
	backpack = SceneObject::LoadMeshFromFile(filename);

	backpack->Move({ 0.0f, 0.0f, -6.5f });
	backpackTranslation = backpack->GetTransform()->GetTranslation();
	objectColor = { 1.0f, 0.5f, 0.31f };

	lightPos = { 0.0f, 2.45f, -5.85f };
	lightColor = { 1.0f, 1.0f, 1.0f };

	// ==================================
	// Skybox
	// ==================================
	skybox = new Skybox();
}

void PhongShadingDemo::Update(float dt)
{
	// if (!isFocused)
	//{
	//	Draw::StartDrawing();
	//	return;
	// }

	// std::cout << "\r" << dt;

	// ==================================
	// Backpack model
	// ==================================

	Shader::UseShader(shader);
	if (shouldRotate)
	{
		backpack->Rotate(backpackRotationSpeed * dt);
	}

	shader.SendUniformVariable("model", backpack->GetModelToWorld());
	shader.SendUniformVariable("view", camera.BuildViewMatrix());
	shader.SendUniformVariable("projection", view.BuildProjectionMatrix());

	shader.SendUniformVariable("objectColor", objectColor);
	shader.SendUniformVariable("lightPos", lightPos);
	shader.SendUniformVariable("lightColor", lightColor);

	shader.BindTexture("textureDiffuse1", 0, backpack->GetTextureHandle(0));

	backpack->Draw();
	

	// ==================================
	// Skybox
	// ==================================
	Shader::UseShader(skyboxShader);
	const mat4<float>& skyboxView = Matrix4::CutOffTranslation(camera.BuildViewMatrix()); // remove translation from origin view matrix
	skyboxShader.SendUniformVariable("model", skybox->GetModelToWorld());
	skyboxShader.SendUniformVariable("view", skyboxView);
	skyboxShader.SendUniformVariable("projection", view.BuildProjectionMatrix());

	skybox->Draw();
}

void PhongShadingDemo::ResetCamera()
{
	// camera.ResetUp();
}

void PhongShadingDemo::HandleResizeEvent(const int new_width, const int new_height)
{
	Demo::HandleResizeEvent(new_width, new_height);
}

void PhongShadingDemo::HandleKeyPress(KeyboardButton button)
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

void PhongShadingDemo::HandleKeyRelease(KeyboardButton button)
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

void PhongShadingDemo::HandleScrollEvent(float scroll_amount)
{
	Demo::HandleScrollEvent(scroll_amount);
}

void PhongShadingDemo::HandleFocusEvent(bool focused)
{
	Demo::HandleFocusEvent(focused);
}

void PhongShadingDemo::DrawGUI()
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

		// Center text
		//std::string text = "Translation";
		//auto windowWidth = ImGui::GetWindowSize().x;
		//auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

		//ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		//ImGui::Text(text.c_str());

		// translation
		ImGui::SetCursorPosX(120.f);
		ImGui::Text("Translation");
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2);
		ImGui::BeginChild("X", { 100, 100 }, true);
		ImGui::Text("X");
		ImGui::DragFloat("", &backpackTranslation.x, 0.1f);
		ImGui::EndChild();

		ImGui::SetNextWindowPos({ p.x + 150, p.y }, 0, ImVec2(0.4f, 0.0f));
		ImGui::BeginChild("Y", { 100, 100 }, true);
		ImGui::Text("Y");
		ImGui::DragFloat("", &backpackTranslation.y, 0.1f);
		ImGui::EndChild();

		ImGui::SetNextWindowPos({ p.x + 250, p.y }, 0, ImVec2(0.3f, 0.0f));
		ImGui::BeginChild("Z", { 100, 100 }, true);
		ImGui::Text("Z");
		ImGui::DragFloat("", &backpackTranslation.z, 0.1f);
		ImGui::EndChild();
		backpack->GetTransform()->SetTranslation(backpackTranslation);

		// rotation
		ImGui::NewLine();
		ImGui::Text("Rotation");
		p = ImGui::GetCursorScreenPos();
		ImGui::BeginChild("Pitch", { 100, 100 }, true);
		ImGui::Text("Pitch");
		ImGui::DragFloat("", &backpackRotationOffset.x, 0.1f);
		ImGui::EndChild();

		ImGui::SetNextWindowPos({ p.x + 150, p.y }, 0, ImVec2(0.4f, 0.0f));
		ImGui::BeginChild("Yaw", { 100, 100 }, true);
		ImGui::Text("Yaw");
		ImGui::DragFloat("", &backpackRotationOffset.y, 0.1f);
		ImGui::EndChild();

		ImGui::SetNextWindowPos({ p.x + 250, p.y }, 0, ImVec2(0.3f, 0.0f));
		ImGui::BeginChild("Roll", { 100, 100 }, true);
		ImGui::Text("Roll");
		ImGui::DragFloat("", &backpackRotationOffset.z, 0.1f);
		ImGui::EndChild();

		ImGui::PopStyleVar();

		if (ImGui::Button("Set", { 50, 20 }))
		{
			backpack->GetTransform()->SetRotation(backpackRotationOffset);
		}
		ImGui::End();
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

		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void PhongShadingDemo::RenderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
