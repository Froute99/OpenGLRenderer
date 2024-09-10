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
#include "Draw.hpp"
#include "PATH.hpp"
#include "Screenshot.hpp"
#include "GameObject.h"
#include "Mesh3D.h" 
#include <Angle.hpp>
#include <vec3.hpp>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void ShapeDrawingDemo::Initialize()
{
	shader.LoadShaderFrom(PATH::shape_vert, PATH::shape_frag);
	lightCubeShader.LoadShaderFrom(PATH::lightCubeVS, PATH::lightCubeFS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	Assimp::Importer importer;

	const std::string& filename = "../assets/phong_cube.fbx";
	unsigned int flag = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;

	testCube = importer.ReadFile(filename, flag);

	vec3<float> cubePos(0.0f, 0.0f, 3.0f);
	objectColor = { 1.0f, 0.5f, 0.31f };
	simpleCube = GameObject::CreateCube(cubePos, { 0.f }, 1.0f, { objectColor, 1.0f });

	lightPos = { 0.7f, 0.4f, 0.8f };
	lightColor = { 1.0f, 1.0f, 1.0f };
	lightCube = GameObject::CreateCube(lightPos, 0.0f, 0.2f, { lightColor, 1.0f });

	// uniform variable location
	uniformModelLocation = glGetUniformLocation(shader.GetHandleToShader(), "model");
	uniformViewLocation = glGetUniformLocation(shader.GetHandleToShader(), "view");
	uniformProjectionLocation = glGetUniformLocation(shader.GetHandleToShader(), "projection");

	uniformObjectColorLocation = glGetUniformLocation(shader.GetHandleToShader(), "objectColor");
	uniformLightPosLocation = glGetUniformLocation(shader.GetHandleToShader(), "lightPos");
	uniformLightColorLocation = glGetUniformLocation(shader.GetHandleToShader(), "lightColor");

	uniformLightCubeModel = glGetUniformLocation(lightCubeShader.GetHandleToShader(), "model");
	uniformLightCubeView = glGetUniformLocation(lightCubeShader.GetHandleToShader(), "view");
	uniformLightCubeProjection = glGetUniformLocation(lightCubeShader.GetHandleToShader(), "projection");

	angle = 1.f;
}

void ShapeDrawingDemo::Update(float dt)
{
	//if (!isFocused)
	//{
	//	Draw::StartDrawing();
	//	return;
	//}

	//std::cout << "\r" << dt;

	//if (testCube->HasMeshes())
	//{
	//	std::cout << testCube->mMeshes[0][0].mVertices[0].x;
	//	std::cout << testCube->mMeshes[0][0].mVertices[0].y;
	//	std::cout << testCube->mMeshes[0][0].mVertices[0].z << std::endl;
	//}

	// camera.Rotate(rotationSpeed);
	// camera.MoveRight(moveSpeed.x);
	// camera.MoveUp(moveSpeed.y);
	// camera.MoveFront(moveSpeed.y);

	Draw::StartDrawing();

	//==================================
	// Simple Cube
	//==================================
	simpleCube->Rotate({ 0.f, ANGLE::DegreeToRadian(35.f * angle * dt), 0.f });
	const mat4<float>& Model = simpleCube->GetModelToWorld();
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

	simpleCube->Draw();

	//==================================
	// Light Cube
	//==================================
	const mat4<float>& LightCubeModel = lightCube->GetModelToWorld();
	Shader::UseShader(lightCubeShader);
	glUniformMatrix4fv(uniformLightCubeModel, 1, GL_FALSE, &LightCubeModel.elements[0][0]);
	glUniformMatrix4fv(uniformLightCubeView, 1, GL_FALSE, &View.elements[0][0]);
	glUniformMatrix4fv(uniformLightCubeProjection, 1, GL_FALSE, &Projection.elements[0][0]);

	lightCube->Draw();

	Draw::FinishDrawing();
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
