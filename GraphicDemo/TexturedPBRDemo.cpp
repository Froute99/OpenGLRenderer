
#include "TexturedPBRDemo.h"
#include "GameObject.h"
#include <glew.h>		// glUniform
#include <iostream>		// error logging
#include <Graphics/Draw.h>	// rendering objects
#include <Graphics/PATH.h>	// path for shaders
#include <Graphics/Texture.h>		// for HDR framebuffer

// imguis
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

void TexturedPBRDemo::Initialize()
{
	pbrShader.LoadShaderFrom("../assets/shaders/PBR_Textured.vert", "../assets/shaders/PBR_Textured.frag");
	hdrShader.LoadShaderFrom("../assets/shaders/HDR.vert", "../assets/shaders/HDR.frag");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	// floating point framebuffer
	glGenFramebuffers(1, &hdrFBO);
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	// mind that the internal format is GL_FLOAT
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GetScreenWidth(), GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
	glBindTexture(GL_TEXTURE_2D, 0);

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

	const std::string& filename = "../assets/Models/backpack.obj";
	backpack = GameObject::LoadMeshFromFile(filename);
	backpack->Move({ 0.f, 0.f, 5.0f });

	sphere = GameObject::CreateSphere({ 0, 0, 3 });
	//const std::string& spherePath = "../assets/sphere.fbx";
	//sphere = GameObject::LoadMeshFromFile(spherePath);
	//sphere->SetObjectType(ObjectType::NonTextured);
	//sphere->Move({ 0.0f, 0.0f, 5.0f });

	uniformModelLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "model");
	uniformViewLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "view");
	uniformProjectionLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "projection");

	albedoMapLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "albedoMap");
	metallicMapLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "metallicMap");
	roughnessMapLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "roughnessMap");
	normalMapLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "normalMap");
	aoMapLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "aoMap");

	albedoMap = new Texture();
	normalMap = new Texture();
	metallicMap = new Texture();
	roughnessMap = new Texture();
	aoMap = new Texture();

	albedoMap->LoadFromPath("../assets/Models/rustediron2_basecolor.png");
	metallicMap->LoadFromPath("../assets/Models/rustediron2_metallic.png");
	roughnessMap->LoadFromPath("../assets/Models/rustediron2_roughness.png");
	normalMap->LoadFromPath("../assets/Models/rustediron2_normal.png");
	//albedoMap->LoadFromPath("../assets/Models/diffuse.jpg");
	//metallicMap->LoadFromPath("../assets/Models/specular.jpg");
	//roughnessMap->LoadFromPath("../assets/Models/roughness.jpg");
	//normalMap->LoadFromPath("../assets/Models/normal.png");
	//aoMap->LoadFromPath("../assets/Models/ao.jpg");

	lightPosLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "lightPositions");
	lightColLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "lightColors");
	camPosLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "camPos");

	//gammaLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "gammaCorrection");

	if (lightPosLocation == -1)
	{
		std::cout << "There's no uniform variable named \"lightPositions\"" << std::endl;
	}
	if (lightColLocation == -1)
	{
		std::cout << "There's no uniform variable named \"lightColors\"" << std::endl;
	}

	lightPos = { 0.f, 3.f, 7.5f };
	lightCol = { 100.f, 100.f, 100.f };


	//lightPos[0] = { 0.00f, 0.4f, 4.0f };
	//lightPos[1] = { 0.25f, 0.4f, 4.0f };
	//lightPos[2] = { 0.50f, 0.4f, 4.0f };
	//lightPos[3] = { 0.75f, 0.4f, 4.0f };

	//lightCol[0] = { 1.0f, 1.0f, 1.0f };
	//lightCol[1] = { 1.0f, 1.0f, 1.0f };
	//lightCol[2] = { 1.0f, 1.0f, 1.0f };
	//lightCol[3] = { 1.0f, 1.0f, 1.0f };
}

void TexturedPBRDemo::Update(float /*dt*/)
{
	camera.MoveX(moveSpeed.x);
	camera.MoveY(moveSpeed.y);
	camera.MoveZ(moveSpeed.z);

	Draw::StartDrawing({ 0.0f, 0.12f, 0.2f });

	//glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Shader::UseShader(pbrShader);

		const mat4<float>& Model = backpack->GetModelToWorld();
		const mat4<float>& View = camera.BuildViewMatrix();
		const mat4<float>& Projection = view.BuildProjectionMatrix();
		glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, &Model.elements[0][0]);
		glUniformMatrix4fv(uniformViewLocation, 1, GL_FALSE, &View.elements[0][0]);
		glUniformMatrix4fv(uniformProjectionLocation, 1, GL_FALSE, &Projection.elements[0][0]);

		// lights
		glUniform3fv(lightPosLocation, 1, &lightPos.x);
		glUniform3fv(lightColLocation, 1, &lightCol.x);

		//glUniform1f(gammaLocation, gamma); // whether perform a gamma correction or not

		// camera position
		vec3<float> camPos = camera.GetEyePosition();
		glUniform3fv(camPosLocation, 1, &camPos.x);

		// materials
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedoMap->GetTexturehandle());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, metallicMap->GetTexturehandle());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, roughnessMap->GetTexturehandle());
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, normalMap->GetTexturehandle());
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, aoMap->GetTexturehandle());

		sphere->Draw();
		//backpack->Draw();
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

		// uniform vec3  albedo;
		// uniform float roughness;
		// uniform float ao;
		// uniform float metallic;
		ImGui::NewLine();

		ImGui::Checkbox("Gamma Correction", &shouldGammaCorrected);
		if (shouldGammaCorrected)
			gamma = 2.2f;
		else
			gamma = 1.0f;

		ImGui::End();
	}
	// lights
	{
		ImGui::Begin("Lights");
		ImGui::SetWindowCollapsed(false);

		ImGui::NewLine();
		ImGui::DragFloat3("Light Position", &lightPos.x, 0.02f);
		ImGui::ColorEdit3("Light Color", &lightCol.x);


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
