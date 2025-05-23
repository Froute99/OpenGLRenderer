
#include "TexturedPBRDemo.h"
#include "GameObject.h"
#include <iostream>				// error logging
#include <Graphics/Draw.h>		// rendering objects
#include <Graphics/PATH.h>		// path for shaders
//#include <Graphics/Texture.h>	// for HDR framebuffer // already in the header

// imguis
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// stbi
#include "stb_image.h"

#include <glew.h>				// glUniform
void TexturedPBRDemo::Initialize()
{
	pbrShader.LoadShaderFrom("../assets/shaders/PBR_Textured.vert", "../assets/shaders/PBR_Textured.frag");
	hdrShader.LoadShaderFrom("../assets/shaders/HDR.vert", "../assets/shaders/HDR.frag");

	//// floating point framebuffer
	//glGenFramebuffers(1, &hdrFBO);
	//glGenTextures(1, &colorBuffer);
	//glBindTexture(GL_TEXTURE_2D, colorBuffer);
	//// mind that the internal format is GL_FLOAT
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GetScreenWidth(), GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//unsigned int rboDepth;
	//glGenRenderbuffers(1, &rboDepth);
	//glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GetScreenWidth(), GetScreenHeight());

	//glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//{
	//	std::cout << "Framebuffer not completed. Requirements are written in the code as a comment. Check them." << std::endl;
	//	/*
	//		We have to attach at least one buffer (color, depth or stencil buffer).
	//		There should be at least one color attachment.
	//		All attachments should be complete as well (reserved memory).
	//		Each buffer should have the same number of samples.
	//	*/
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//const std::string& filename = "../assets/Models/backpack.obj";
	//backpack = GameObject::LoadMeshFromFile(filename);
	//backpack->Move({ 0.f, 0.f, -5.0f });

	sphere = GameObject::CreateSphere({ 0, 0, 0 });
	sphere->Move({ 0.f, 0.f, -5.f });

	albedoMap = new Texture();
	normalMap = new Texture();
	metallicMap = new Texture();
	roughnessMap = new Texture();
	aoMap = new Texture();

	albedoMap->LoadFromPath("../assets/Models/rustediron2_basecolor.png", true);
	metallicMap->LoadFromPath("../assets/Models/rustediron2_metallic.png");
	roughnessMap->LoadFromPath("../assets/Models/rustediron2_roughness.png");
	normalMap->LoadFromPath("../assets/Models/rustediron2_normal.png");

	lightPos = { 0.34f, 0.20f, -4.00f };
	lightColor = { 1.f };

	//lightPos[0] = { 0.00f, 0.4f, 4.0f };
	//lightPos[1] = { 0.25f, 0.4f, 4.0f };
	//lightPos[2] = { 0.50f, 0.4f, 4.0f };
	//lightPos[3] = { 0.75f, 0.4f, 4.0f };

	//lightCol[0] = { 1.0f, 1.0f, 1.0f };
	//lightCol[1] = { 1.0f, 1.0f, 1.0f };
	//lightCol[2] = { 1.0f, 1.0f, 1.0f };
	//lightCol[3] = { 1.0f, 1.0f, 1.0f };

	//// Framebuffer setup for HDR image cubemap
	//unsigned int captureFBO, captureRBO;
	//glGenFramebuffers(1, &captureFBO);
	//glGenRenderbuffers(1, &captureRBO);

	//glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, GetScreenWidth(), GetScreenHeight());
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	//// Load HDR image
	//// Later, figure out what exactly internalformat and format in member function of Texture class
	//int			 width, height, nrComponents;
	//float*		 data = stbi_loadf("newport_loft.hdr", &width, &height, &nrComponents, 0);
	//unsigned int hdrTexture;
	//if (data)
	//{
	//	glGenTextures(1, &hdrTexture);
	//	glBindTexture(GL_TEXTURE_2D, hdrTexture);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	stbi_image_free(data);
	//}
	//else
	//{
	//	std::cout << "Failed to load HDR image." << std::endl;
	//}

	//unsigned int envCubemap;
	//glGenTextures(1, &envCubemap);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	//for (unsigned int i = 0; i < 6; ++i)
	//{
	//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, GetScreenWidth(), GetScreenHeight(), 0, GL_RGB, GL_FLOAT, nullptr);
	//}
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//mat4<float> captureProjection = view.BuildProjectionMatrix();
	//mat4<float> captureViews[] = {		// may swap Z and -Z
	//	Matrix4::BuildLookAt(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(1.0f, 0.0f, 0.0f), vec3<float>(0.0f, -1.0f, 0.0f)),
	//	Matrix4::BuildLookAt(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(-1.0f, 0.0f, 0.0f), vec3<float>(0.0f, -1.0f, 0.0f)),
	//	Matrix4::BuildLookAt(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(0.0f, 1.0f, 0.0f), vec3<float>(0.0f, 0.0f, 1.0f)),
	//	Matrix4::BuildLookAt(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(0.0f, -1.0f, 0.0f), vec3<float>(0.0f, 0.0f, -1.0f)),
	//	Matrix4::BuildLookAt(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(0.0f, 0.0f, 1.0f), vec3<float>(0.0f, -1.0f, 0.0f)),
	//	Matrix4::BuildLookAt(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(0.0f, 0.0f, -1.0f), vec3<float>(0.0f, -1.0f, 0.0f))
	//};

	//// Mapping equirectangular to cubemap in shader (refer my calculation in paper note)
	//equirectangularMappingShader.LoadShaderFrom("../assets/shaders/equirectangularMappingCubemap.vs", "../assets/shaders/equirectangularMappingCubemap.fs");
	//Shader::UseShader(equirectangularMappingShader);
	//glUniform1i(glGetUniformLocation(equirectangularMappingShader.GetHandleToShader(), "equirectangularMap"), 0);
	//glUniformMatrix4fv(glGetUniformLocation(equirectangularMappingShader.GetHandleToShader(), "projection"), 1, GL_FALSE, &captureProjection.elements[0][0]);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, hdrTexture);

	//glViewport(0, 0, GetScreenWidth(), GetScreenHeight());
	//glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	//for (unsigned int i = 0; i < 6; ++i)
	//{
	//	glUniformMatrix4fv(glGetUniformLocation(equirectangularMappingShader.GetHandleToShader(), "view"), 1, GL_FALSE, &captureViews[i].elements[0][0]);
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	RenderCube(); // renders a 1x1 cube
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TexturedPBRDemo::Update(float /*dt*/)
{
	camera.MoveX(moveSpeed.x);
	camera.MoveY(moveSpeed.y);
	camera.MoveZ(moveSpeed.z);

	Draw::StartDrawing();

	//glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Shader::UseShader(pbrShader);

		const mat4<float>& Model = sphere->GetModelToWorld();
		const mat4<float>& View = camera.BuildViewMatrix();
		const mat4<float>& Projection = view.BuildProjectionMatrix();
		pbrShader.SendUniformVariable("model", Model);
		pbrShader.SendUniformVariable("view", View);
		pbrShader.SendUniformVariable("projection", Projection);

		// lights
		pbrShader.SendUniformVariable("lightPositions", lightPos);
		pbrShader.SendUniformVariable("lightColors", lightColor);

		// camera position
		vec3<float> camPos = camera.GetEyePosition();
		pbrShader.SendUniformVariable("camPos", camPos);

		// materials
		pbrShader.BindTexture("albedoMap", 0, albedoMap->GetTexturehandle());
		pbrShader.BindTexture("metallicMap", 1, metallicMap->GetTexturehandle());
		pbrShader.BindTexture("roughnessMap", 2, roughnessMap->GetTexturehandle());
		pbrShader.BindTexture("normalMap", 3, normalMap->GetTexturehandle());
		pbrShader.BindTexture("aoMap", 4, aoMap->GetTexturehandle());

		sphere->Draw();
	}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glClear(GL_COLOR_BUFFER_BIT);
	//Shader::UseShader(hdrShader);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, colorBuffer);
	//glUniform1i(glGetUniformLocation(hdrShader.GetHandleToShader(), "hdr"), hdr);
	//glUniform1f(glGetUniformLocation(hdrShader.GetHandleToShader(), "exposure"), exposure);
	//RenderQuad();

	Draw::FinishDrawing();

	ImguiHelper();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TexturedPBRDemo::ResetCamera()
{
}

void TexturedPBRDemo::HandleResizeEvent(const int& new_width, const int& new_height)
{
	new_width;
	new_height;
}

void TexturedPBRDemo::HandleKeyPress(KeyboardButton button)
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
	}
}

void TexturedPBRDemo::HandleKeyRelease(KeyboardButton button)
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

void TexturedPBRDemo::HandleScrollEvent(float scroll_amount)
{
	scroll_amount;
}

void TexturedPBRDemo::HandleFocusEvent(bool focused)
{
	focused;
}

void TexturedPBRDemo::ImguiHelper()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();

	// Global
	{
		ImGui::Begin("Global");
		ImGui::SetWindowCollapsed(false);

		ImGui::Text("PBR Demo");

		ImGui::End();
	}
	// PBR properties
	{
		ImGui::Begin("PBR Properties");
		ImGui::SetWindowCollapsed(false);

		if (ImGui::DragFloat3("Rotation", &rotationOffset.x, 0.1f))
		{
			backpack->GetTransform()->SetRotation(rotationOffset);
		}

		ImGui::End();
	}
	// lights
	{
		ImGui::Begin("Lights");
		ImGui::SetWindowCollapsed(false);

		ImGui::NewLine();
		ImGui::DragFloat3("Light Position", &lightPos.x, 0.02f);
		ImGui::ColorEdit3("Light Color", &lightColor.x);

		ImGui::NewLine();

		//ImGui::DragFloat3("Light 1 Position", &lightPos[0].x, 0.02f);
		//ImGui::ColorEdit3("Light 1 Color", &lightCol[0].x);

		//ImGui::DragFloat3("Light 2 Position", &lightPos[1].x, 0.02f);
		//ImGui::ColorEdit3("Light 2 Color", &lightCol[1].x);

		//ImGui::DragFloat3("Light 3 Position", &lightPos[2].x, 0.02f);
		//ImGui::ColorEdit3("Light 3 Color", &lightCol[2].x);

		//ImGui::DragFloat3("Light 4 Position", &lightPos[3].x, 0.02f);
		//ImGui::ColorEdit3("Light 4 Color", &lightCol[3].x);
	}

	ImGui::Render();
}

void TexturedPBRDemo::RenderQuad()
{
	if (quadVAO == 0)
	{
		// For FBO
		float quadVertices[] = {
			// positions        // texture Coords
			1.0f,
			1.0f,
			0.0f,
			1.0f,
			1.0f,
			1.0f,
			-1.0f,
			0.0f,
			1.0f,
			0.0f,
			-1.0f,
			1.0f,
			0.0f,
			0.0f,
			1.0f,
			-1.0f,
			-1.0f,
			0.0f,
			0.0f,
			0.0f,
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
