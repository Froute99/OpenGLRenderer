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

#include <Graphics/Image.hpp>
#include "stb_image.h"
#include "stb_image_write.h"

float skyboxVertices[] = {
	// positions
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};

void ShapeDrawingDemo::Initialize()
{
	shader.LoadShaderFrom(PATH::shape_vert, PATH::shape_frag);
	skyboxShader.LoadShaderFrom(PATH::skyboxVS, PATH::skyboxFS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	const std::string& filename = "../assets/backpack/backpack.obj";
	backpack = GameObject::LoadMeshFromFile(filename);

	vec3<float> cubePos(0.0f, 0.0f, 5.0f);
	cubeTranslation = cubePos;
	backpack->Move(cubePos);
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

	uniformLightCubeModel = glGetUniformLocation(skyboxShader.GetHandleToShader(), "model");
	uniformLightCubeView = glGetUniformLocation(skyboxShader.GetHandleToShader(), "view");
	uniformLightCubeProjection = glGetUniformLocation(skyboxShader.GetHandleToShader(), "projection");

	// ==================================
	// cube map
	// ==================================

	// create mesh
	// ==================================
	cube = GameObject::CreateCube({ 0.f }, { 0.f }, 1.0f);
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// load textures
	// ==================================
	// since using Y up coordinate system
	//	GL_TEXTURE_CUBE_MAP_POSITIVE_X Right
	//	GL_TEXTURE_CUBE_MAP_NEGATIVE_X Left
	//	GL_TEXTURE_CUBE_MAP_POSITIVE_Y Top
	//	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y Bottom
	//	GL_TEXTURE_CUBE_MAP_POSITIVE_Z Front
	//	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z Back
	// ==================================
	cubemapPaths.push_back("../assets/skybox/right.jpg");
	cubemapPaths.push_back("../assets/skybox/left.jpg");
	cubemapPaths.push_back("../assets/skybox/top.jpg");
	cubemapPaths.push_back("../assets/skybox/bottom.jpg");
	cubemapPaths.push_back("../assets/skybox/front.jpg");
	cubemapPaths.push_back("../assets/skybox/back.jpg");

	glGenTextures(1, &cubemapTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);

	int w, h, nrChannels;
	for (unsigned int i = 0; i < cubemapPaths.size(); i++)
	{
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(cubemapPaths[i].c_str(), &w, &h, &nrChannels, 0);
		if (data)
		{
			std::cout << "succeed to load " << cubemapPaths[i] << std::endl;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
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

	Draw::StartDrawing();

	// ==================================
	// Simple Cube
	// ==================================
	backpack->Rotate({ 0.f, ANGLE::DegreeToRadian(35.f * angle * dt), 0.f });
	const mat4<float>& Model = backpack->GetModelToWorld();
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
	glBindTexture(GL_TEXTURE_2D, backpack->GetTextureHandle(0));

	backpack->Draw();
	glActiveTexture(GL_TEXTURE0);

	// ==================================
	// Skybox
	// ==================================
	glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
	Shader::UseShader(skyboxShader);
	const mat4<float>& LightCubeModel = cube->GetModelToWorld();
	const mat4<float>& skyboxView = Matrix4::CutOffTranslation(View); // remove translation from origin view matrix
	glUniformMatrix4fv(uniformLightCubeModel, 1, GL_FALSE, &LightCubeModel.elements[0][0]);
	glUniformMatrix4fv(uniformLightCubeView, 1, GL_FALSE, &skyboxView.elements[0][0]);
	glUniformMatrix4fv(uniformLightCubeProjection, 1, GL_FALSE, &Projection.elements[0][0]);

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

	Draw::FinishDrawing();

	ImguiHelper();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

		ImGui::Text("Demo."); // Display some text (you can use a format strings too)

		ImGui::ColorEdit3("Light color", (float*)&lightColor); // Edit 3 floats representing a color
		ImGui::ColorEdit3("Cube color", (float*)&objectColor); // Edit 3 floats representing a

		ImGui::NewLine();
		ImGui::DragFloat3("Drag Float3", &cubeTranslation.x, 0.1f);
		cube->GetTransform()->SetTranslation(cubeTranslation);

		ImGui::End();
	}

	// Rendering
	ImGui::Render();
}
