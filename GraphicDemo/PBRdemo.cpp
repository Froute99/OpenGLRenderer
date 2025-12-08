/*
 *	Author: JeongHak Kim
 *	File_name: EnvironmentMap.cpp
 *
 *	Diffuse Irradiance, IBL
 *
 *	2024. 12. 14
 */

#include "PBRdemo.h"
#include "SceneObject.h"
#include <glew.h>		// glUniform
#include <Graphics/Draw.h>	// rendering objects
#include <Graphics/PATH.h>	// path for shaders
#include <Graphics/Texture.h>		// textures

// imguis
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>		// error logging

#include <ShaderManager.h>

PBRDemo::~PBRDemo()
{
	delete sphere1;
	sphere1 = nullptr;
	delete sphere2;
	sphere2 = nullptr;
	delete ironSphere;
	ironSphere = nullptr;

	delete albedoMap;
	delete metallicMap;
	delete roughnessMap;
	delete normalMap;

	albedoMap = nullptr;
	metallicMap = nullptr;
	roughnessMap = nullptr;
	normalMap = nullptr;

}

void PBRDemo::Initialize()
{
	Shader* pbrShader = ShaderManager::GetShader(ShaderDefinition::PBR);
	Shader* texturedPbrShader = ShaderManager::GetShader(ShaderDefinition::TexturedPBR);
	//pbrShader.LoadShaderFrom("../assets/shaders/PBR.vert", "../assets/shaders/PBR.frag");
	//texturedShader.CanLoadShader("../assets/shaders/PBR_Textured.vert", "../assets/shaders/PBR_Textured.frag");
	if (!envMap.CanLoad("../assets/newport_loft.hdr", view.BuildProjectionMatrix()))
	{
		std::cout << "Failed to load env map\n";
	}

	sphere1 = SceneObject::CreateSphere({ 0, 0, 0 });
	sphere1->SetObjectType(ObjectType::NonTextured);
	sphere1->Move({ 0.f, 0.f, -4.f });

	sphere2 = SceneObject::CreateSphere({ 0, 0, 0 });
	sphere2->SetObjectType(ObjectType::NonTextured);
	sphere2->Move({ 2.5f, 0.f, -4.f });

	ironSphere = SceneObject::CreateSphere({ 0, 0, 0 });
	ironSphere->Move({ -2.5f, 0.f, -4.f });

	albedoMap = new Texture();
	metallicMap = new Texture();
	roughnessMap = new Texture();
	normalMap = new Texture();
	//aoMap = new Texture();

	albedoMap->LoadFromPath("../assets/Models/rustediron2_basecolor.png", true);
	metallicMap->LoadFromPath("../assets/Models/rustediron2_metallic.png");
	roughnessMap->LoadFromPath("../assets/Models/rustediron2_roughness.png");
	normalMap->LoadFromPath("../assets/Models/rustediron2_normal.png");

	matricesBlock.BindTo(pbrShader->GetHandleToShader(), "Matrices");
	matricesBlock.BindTo(texturedPbrShader->GetHandleToShader(), "Matrices");
	lightsBlock.BindTo(pbrShader->GetHandleToShader(), "Lights");

	sphereColor = vec3<float>(1.f, 0.f, 0.f);
	roughness = 0.1f;
	ambientOcclusion = 0.1f;
	metallic = 0.8f;

	lightPosition = { 3.6f, 0.55f, -0.4f };
	lightColor = { 1.f };
	lightIntensity = 10.f;
}

void PBRDemo::Update(float /*dt*/)
{
	const vec3<float>& camPos = camera.GetEyePosition();
	lightsBlock.WriteData(0, 12, &lightPosition[0]);
	lightsBlock.WriteData(16, 12, &lightColor[0]);
	lightsBlock.WriteData(28, 4, &lightIntensity);
	lightsBlock.WriteData(32, 12, &camPos[0]);

	const mat4<float>& View = camera.BuildViewMatrix();
	const mat4<float>& Projection = view.BuildProjectionMatrix();
	matricesBlock.WriteData(0, 64, &View);
	matricesBlock.WriteData(64, 64, &Projection);

	Shader* pbrShader = ShaderManager::GetShader(ShaderDefinition::PBR);
	pbrShader->Use();
	envMap.BindIBLTexture(pbrShader);

	pbrShader->SendUniformVariable("model", sphere1->GetModelToWorld());

	pbrShader->SendUniformVariable("albedo", sphereColor);
	pbrShader->SendUniformVariable("roughness", roughness);
	pbrShader->SendUniformVariable("ao", ambientOcclusion);
	pbrShader->SendUniformVariable("metallic", metallic);

	pbrShader->SendUniformVariable("lightPositions", lightPosition);
	pbrShader->SendUniformVariable("lightColors", lightColor);
	pbrShader->SendUniformVariable("lightIntensity", lightIntensity);
	pbrShader->SendUniformVariable("camPos", camPos);

	sphere1->Draw();

	pbrShader->SendUniformVariable("model", sphere2->GetModelToWorld());

	float		roughness2 = 0.8f;
	float		metallic2 = 0.1f;
	pbrShader->SendUniformVariable("roughness", roughness2);
	pbrShader->SendUniformVariable("ao", ambientOcclusion);
	pbrShader->SendUniformVariable("metallic", metallic2);

	pbrShader->SendUniformVariable("lightPositions", lightPosition);
	pbrShader->SendUniformVariable("lightColors", lightColor);
	pbrShader->SendUniformVariable("lightIntensity", lightIntensity);
	pbrShader->SendUniformVariable("camPos", camera.GetEyePosition());

	sphere2->Draw();

	Shader* texturedPbrShader = ShaderManager::GetShader(ShaderDefinition::TexturedPBR);
	texturedPbrShader->Use();
	envMap.BindIBLTexture(texturedPbrShader);
	texturedPbrShader->BindTexture("albedoMap", 3, albedoMap->GetTexturehandle());
	texturedPbrShader->BindTexture("metallicMap", 4, metallicMap->GetTexturehandle());
	texturedPbrShader->BindTexture("roughnessMap", 5, roughnessMap->GetTexturehandle());
	texturedPbrShader->BindTexture("normalMap", 6, normalMap->GetTexturehandle());

	texturedPbrShader->SendUniformVariable("model", ironSphere->GetModelToWorld());

	texturedPbrShader->SendUniformVariable("lightPositions", lightPosition);
	texturedPbrShader->SendUniformVariable("lightColors", lightColor);
	texturedPbrShader->SendUniformVariable("camPos", camera.GetEyePosition());

	ironSphere->Draw();

	envMap.Render(Matrix4::CutOffTranslation(View), Projection);
}

void PBRDemo::ResetCamera()
{

}

void PBRDemo::HandleResizeEvent(const int new_width, const int new_height)
{
	Demo::HandleResizeEvent(new_width, new_height);
}

void PBRDemo::HandleKeyPress(KeyboardButton button)
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

void PBRDemo::HandleKeyRelease(KeyboardButton button)
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

void PBRDemo::HandleScrollEvent(float scroll_amount)
{
	scroll_amount;
}

void PBRDemo::HandleFocusEvent(bool focused)
{
	focused;
}

void PBRDemo::HandleMousePositionEvent(float x, float y)
{
	Demo::HandleMousePositionEvent(x, y);
}

void PBRDemo::DrawGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();

	// Global
	{
		ImGui::Begin("Global");
		ImGui::SetWindowCollapsed(false);

		ImGui::Text("PBR Demo");
		ImGui::Text("Multiple materials");
		ImGui::NewLine();
		ImGui::Text("Controls:");
		ImGui::Text("Mouse movement - Look around");
		ImGui::Text("WASD - Move around");
		ImGui::Text("QE - Go up and down");
		ImGui::Text("Page up & down - Convert current showing demo stage");

		ImGui::End();
	}
	// PBR properties
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize;
		ImGui::Begin("PBR Properties", NULL, flags);
		ImGui::SetWindowCollapsed(false);
		ImGui::SetWindowSize({ 300, 150 });

		ImGui::NewLine();
		ImGui::ColorEdit3("albedo", &sphereColor.x);

		ImGui::DragFloat("roughness", &roughness, 0.005f, 0.000001f, 1.0f);
		ImGui::DragFloat("ambient occlusion", &ambientOcclusion, 0.01f, 0.000001f, 1.0f);
		ImGui::DragFloat("metallic", &metallic, 0.005f, 0.000001f, 1.0f);

		ImGui::End();
	}
	// lights
	{
		ImGui::Begin("Lights");
		ImGui::SetWindowCollapsed(false);

		ImGui::NewLine();
		ImGui::DragFloat3("Light 1 Position", &lightPosition.x, 0.02f);
		ImGui::ColorEdit3("Light 1 Color", &lightColor.x);

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void PBRDemo::RenderQuad()
{
	if (quadVAO == 0)
	{
		// For FBO
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

