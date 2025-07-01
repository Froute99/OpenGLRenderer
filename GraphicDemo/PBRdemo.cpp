
#include "PBRdemo.h"
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


void PBRDemo::Initialize()
{
	pbrShader.LoadShaderFrom("../assets/shaders/PBR.vert", "../assets/shaders/PBR.frag");

	sphere1 = GameObject::CreateSphere({ 0, 0, 0 });
	sphere1->SetObjectType(ObjectType::NonTextured);
	sphere1->Move({ 0.f, 0.f, -4.f });

	sphere2 = GameObject::CreateSphere({ 0, 0, 0 });
	sphere2->SetObjectType(ObjectType::NonTextured);
	sphere2->Move({ 2.5f, 0.f, -4.f });

	lightPosLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "lightPositions");
	lightColLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "lightColors");

	if (lightPosLocation == -1)
	{
		std::cout << "There's no uniform variable named \"lightPositions\"" << std::endl;
	}
	if (lightColLocation == -1)
	{
		std::cout << "There's no uniform variable named \"lightColors\"" << std::endl;
	}

	if (!envMap.CanLoad("../assets/newport_loft.hdr", view.BuildProjectionMatrix()))
	{
		std::cout << "Failed to load env map\n";
	}

	sphereColor = vec3<float>(1.f, 0.f, 0.f);
	roughness = 0.1f;
	ambientOcclusion = 0.1f;
	metallic = 0.8f;

	lightPos[0] = { 3.6f, 0.55f, -0.4f };
	lightPos[1] = { 0.f };
	lightPos[2] = { 0.f };
	lightPos[3] = { 0.f };

	lightCol[0] = { 10.f };
	lightCol[1] = { 0.f };
	lightCol[2] = { 0.f };
	lightCol[3] = { 0.f };

}

void PBRDemo::Update(float dt)
{
	Demo::Update(dt);
	Draw::StartDrawing();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader::UseShader(pbrShader);
	pbrShader.SendUniformVariable("irradianceMap", 0);
	pbrShader.SendUniformVariable("prefilterMap", 1);
	pbrShader.SendUniformVariable("brdfLUT", 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envMap.GetIrradianceMapHandle());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envMap.GetPrefilterMapHandle());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, envMap.GetBRDFLUTTextureHandle());


	mat4<float> Model = sphere1->GetModelToWorld();
	const mat4<float>& View = camera.BuildViewMatrix();
	const mat4<float>& Projection = view.BuildProjectionMatrix();

	// This uniform sending bunch should be moved to class method or kind of helper function
	pbrShader.SendUniformVariable("model", Model);
	pbrShader.SendUniformVariable("view", View);
	pbrShader.SendUniformVariable("projection", Projection);

	pbrShader.SendUniformVariable("albedo", sphereColor);
	pbrShader.SendUniformVariable("roughness", roughness);
	pbrShader.SendUniformVariable("ao", ambientOcclusion);
	pbrShader.SendUniformVariable("metallic", metallic);

	sphere1->Draw();

	Model = sphere2->GetModelToWorld();
	pbrShader.SendUniformVariable("model", Model);

	vec3<float> sphereColor2 = { 0.f, 1.f, 0.f };
	float		roughness2 = 0.8f;
	float		metallic2 = 0.1f;
	pbrShader.SendUniformVariable("albedo", sphereColor2);
	pbrShader.SendUniformVariable("roughness", roughness2);
	pbrShader.SendUniformVariable("ao", ambientOcclusion);
	pbrShader.SendUniformVariable("metallic", metallic2);

	sphere2->Draw();

	// lights
	glUniform3fv(lightPosLocation, 4, &lightPos[0].x);
	glUniform3fv(lightColLocation, 4, &lightCol[0].x);

	pbrShader.SendUniformVariable("camPos", camera.GetEyePosition());

	envMap.Render(Matrix4::CutOffTranslation(camera.BuildViewMatrix()), Projection);

	Draw::FinishDrawing();

	ImguiHelper();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

void PBRDemo::ImguiHelper()
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
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize;
		ImGui::Begin("PBR Properties", NULL, flags);
		ImGui::SetWindowCollapsed(false);
		ImGui::SetWindowSize({ 300, 150 });

		ImGui::NewLine();
		ImGui::ColorEdit3("albedo", &sphereColor.x);

		ImGui::DragFloat("roughness", &roughness, 0.005f, 0.000001f, 1.0f);
		ImGui::DragFloat("ambient occlusion", &ambientOcclusion, 0.01f, 0.000001f, 1.0f);
		ImGui::DragFloat("metallic", &metallic, 0.005f, 0.000001f, 1.0f);

		// using this thing to set values preset.
		//bool isModified = ImGui::ListBox("Surface type", &surfaceIndex, surfacesList, sizeof(surfacesList) / sizeof(char*));
		//if (isModified)
		//{
		//	switch (surfaceIndex)
		//	{
		//		default:
		//		case 0: baseReflectivity = { 0.04f }; break;
		//		case 1: baseReflectivity = { 0.04f }; break;
		//		case 2: baseReflectivity = { 0.17f }; break;
		//		case 3: baseReflectivity = { 0.56f, 0.57f, 0.58f }; break;
		//		case 4: baseReflectivity = { 0.95f, 0.64f, 0.54f }; break;
		//		case 5: baseReflectivity = { 1.00f, 0.71f, 0.29f }; break;
		//	}
		//}

		ImGui::End();
	}
	// lights
	{
		ImGui::Begin("Lights");
		ImGui::SetWindowCollapsed(false);

		ImGui::NewLine();
		ImGui::DragFloat3("Light 1 Position", &lightPos[0].x, 0.02f);
		ImGui::ColorEdit3("Light 1 Color", &lightCol[0].x);

		ImGui::DragFloat3("Light 2 Position", &lightPos[1].x, 0.02f);
		ImGui::ColorEdit3("Light 2 Color", &lightCol[1].x);

		ImGui::DragFloat3("Light 3 Position", &lightPos[2].x, 0.02f);
		ImGui::ColorEdit3("Light 3 Color", &lightCol[2].x);

		ImGui::DragFloat3("Light 4 Position", &lightPos[3].x, 0.02f);
		ImGui::ColorEdit3("Light 4 Color", &lightCol[3].x);
	}

	ImGui::Render();
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
