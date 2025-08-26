
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

	sphere = GameObject::CreateSphere({ 0, 0, 0 });
	sphere->Move({ 0.f, 0.f, -5.f });

	Shader::UseShader(pbrShader);
	pbrShader.SendUniformVariable("irradianceMap", 0);

	albedoMap = new Texture();
	normalMap = new Texture();
	metallicMap = new Texture();
	roughnessMap = new Texture();
	aoMap = new Texture();

	//if (!envMap.CanLoad("../assets/brown_photostudio_02_4k.hdr", view.BuildProjectionMatrix()))
	{
		//std::cout << "Failed to load env map\n";
	}
	albedoMap->LoadFromPath("../assets/Models/rustediron2_basecolor.png", true);
	metallicMap->LoadFromPath("../assets/Models/rustediron2_metallic.png");
	roughnessMap->LoadFromPath("../assets/Models/rustediron2_roughness.png");
	normalMap->LoadFromPath("../assets/Models/rustediron2_normal.png");

	lightPos = { 0.34f, 0.20f, -3.20f };
	lightColor = { 1.f };

}

static int	 frameCount = 0;
static float frameTime = 0.f;

void TexturedPBRDemo::Update(float dt)
{
	++frameCount;
	frameTime += dt;
	if (frameTime >= 1.0f)
	{
		frameTime -= 1.0f;
		std::cout << frameCount << std::endl;
		frameCount = 0;
	}

	//glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	{
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
		pbrShader.SendUniformVariable("shouldIrradiance", shouldIrradiance);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envMap.GetIrradianceMapHandle());
		// materials
		pbrShader.BindTexture("albedoMap", 1, albedoMap->GetTexturehandle());
		pbrShader.BindTexture("metallicMap", 2, metallicMap->GetTexturehandle());
		pbrShader.BindTexture("roughnessMap", 3, roughnessMap->GetTexturehandle());
		pbrShader.BindTexture("normalMap", 4, normalMap->GetTexturehandle());
		//pbrShader.BindTexture("aoMap", 5, aoMap->GetTexturehandle());

		sphere->Draw();
	}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//envMap.Render(Matrix4::CutOffTranslation(camera.BuildViewMatrix()));

	//glClear(GL_COLOR_BUFFER_BIT);
	//Shader::UseShader(hdrShader);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, colorBuffer);
	//glUniform1i(glGetUniformLocation(hdrShader.GetHandleToShader(), "hdr"), hdr);
	//glUniform1f(glGetUniformLocation(hdrShader.GetHandleToShader(), "exposure"), exposure);
	//RenderQuad();
}

void TexturedPBRDemo::ResetCamera()
{
}

void TexturedPBRDemo::HandleResizeEvent(const int new_width, const int new_height)
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

void TexturedPBRDemo::DrawGUI()
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

		ImGui::Checkbox("Use Irradiance", &shouldIrradiance);

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
