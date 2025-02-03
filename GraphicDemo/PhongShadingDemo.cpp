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

void PhongShadingDemo::Initialize()
{
	shader.LoadShaderFrom(PATH::texture_vert, PATH::texture_frag);
	skyboxShader.LoadShaderFrom(PATH::skyboxVS, PATH::skyboxFS);
	hdrShader.LoadShaderFrom("../assets/shaders/HDR.vert", "../assets/shaders/HDR.frag");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	const std::string& filename = "../assets/Models/backpack.obj";
	backpack = GameObject::LoadMeshFromFile(filename);

	vec3<float> pos(0.0f, 0.0f, 5.0f);
	backpackTranslation = pos;
	backpack->Move(pos);
	objectColor = { 1.0f, 0.5f, 0.31f };

	lightPos = { 0.0f, 2.45f, 3.85f };
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
	// Skybox
	// ==================================
	skybox = new Skybox();


	// HDR
	// floating point framebuffer
	glGenFramebuffers(1, &hdrFBO);
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	// mind that the internal format is GL_FLOAT
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GetScreenWidth(), GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GetScreenWidth(), GetScreenHeight());

	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer not completed. Requirements are written in the code as a comment. Check them." << std::endl;
		/*
			We have to attach at least one buffer (color, depth or stencil buffer).
			There should be at least one color attachment.
			All attachments should be complete as well (reserved memory).
			Each buffer should have the same number of samples.
		*/
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PhongShadingDemo::Update(float dt)
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

	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ==================================
		// Backpack model
		// ==================================

		Shader::UseShader(shader);
		dt;
		//backpack->Rotate(backpackRotationSpeed * dt);
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
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, backpack->GetTextureHandle(0));

		backpack->Draw();
	}

	//// ==================================
	//// Skybox
	//// ==================================
	// Shader::UseShader(skyboxShader);
	////const mat4<float>& LightCubeModel = cube->GetModelToWorld();
	// const mat4<float>& LightCubeModel = skybox->GetModelToWorld();
	// const mat4<float>& skyboxView = Matrix4::CutOffTranslation(View); // remove translation from origin view matrix
	// glUniformMatrix4fv(uniformLightCubeModel, 1, GL_FALSE, &LightCubeModel.elements[0][0]);
	// glUniformMatrix4fv(uniformLightCubeView, 1, GL_FALSE, &skyboxView.elements[0][0]);
	// glUniformMatrix4fv(uniformLightCubeProjection, 1, GL_FALSE, &Projection.elements[0][0]);

	// skybox->Draw();

	// HDR
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader::UseShader(hdrShader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glUniform1i(glGetUniformLocation(hdrShader.GetHandleToShader(), "hdr"), hdr);
	glUniform1f(glGetUniformLocation(hdrShader.GetHandleToShader(), "exposure"), exposure);
	RenderQuad();

	Draw::FinishDrawing();

	ImguiHelper();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void PhongShadingDemo::ResetCamera()
{
	// camera.ResetUp();
}

void PhongShadingDemo::HandleResizeEvent(const int& new_width, const int& new_height)
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

void PhongShadingDemo::ImguiHelper()
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

		ImGui::NewLine();

		// rotation
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
		backpack->GetTransform()->SetTranslation(backpackTranslation);
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
