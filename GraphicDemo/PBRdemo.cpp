
#include "PBRdemo.h"
#include "GameObject.h"
#include <glew.h>		// glUniform
#include <iostream>		// error logging
#include <Graphics/Draw.hpp>	// rendering objects
#include <Graphics/PATH.hpp>	// path for shaders

void PBRDemo::Initialize()
{
	pbrShader.LoadShaderFrom("../assets/shaders/PBR.vert", "../assets/shaders/PBR.frag");
	//pbrShader.LoadShaderFrom(PATH::shape_vert, PATH::shape_frag);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	const std::string& spherePath = "../assets/sphere.fbx";
	sphere = GameObject::LoadMeshFromFile(spherePath);
	sphere->SetObjectType(ObjectType::NonTextured);
	sphere->Move({ 0.0f, 0.0f, 5.0f });

	uniformModelLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "model");
	uniformViewLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "view");
	uniformProjectionLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "projection");

	//sphereColorLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "objectColor");
	sphereColorLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "albedo");
	roughnessLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "roughness");
	aoLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "ao");
	metallicLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "metallic");

	//lightPosLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "lightPosition");
	lightPosLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "lightPositions");
	//lightColLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "lightColor");
	lightColLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "lightColors");
	camPosLocation = glGetUniformLocation(pbrShader.GetHandleToShader(), "camPos");

	if (lightPosLocation == -1)
	{
		std::cout << "There's no uniform variable named \"lightPositions\"" << std::endl;
	}
	if (lightColLocation == -1)
	{
		std::cout << "There's no uniform variable named \"lightColors\"" << std::endl;
	}

	sphereColor = vec3<float>(1.f, 0.f, 0.f);
	roughness = 0.8f;
	ambientOcclusion = 0.1f;
	metallic = 0.01f;

	lightPos[0] = vec3<float>(1.0f, 1.0f, 4.0f);
	lightPos[1] = vec3<float>(1.0f, 1.0f, 4.0f);
	lightPos[2] = vec3<float>(1.0f, 1.0f, 4.0f);
	lightPos[3] = vec3<float>(1.0f, 1.0f, 4.0f);

	lightCol[0] = vec3<float>(1.0f, 1.0f, 1.0f);
	lightCol[1] = vec3<float>(1.0f, 1.0f, 1.0f);
	lightCol[2] = vec3<float>(1.0f, 1.0f, 1.0f);
	lightCol[3] = vec3<float>(1.0f, 1.0f, 1.0f);
}

void PBRDemo::Update(float /*dt*/)
{
	Draw::StartDrawing({0.0f, 0.12f, 0.2f});

	Shader::UseShader(pbrShader);

	const mat4<float>& Model = sphere->GetModelToWorld();
	const mat4<float>& View = camera.BuildViewMatrix();
	const mat4<float>& Projection = view.BuildProjectionMatrix();
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, &Model.elements[0][0]);
	glUniformMatrix4fv(uniformViewLocation, 1, GL_FALSE, &View.elements[0][0]);
	glUniformMatrix4fv(uniformProjectionLocation, 1, GL_FALSE, &Projection.elements[0][0]);

	// uniform variables
	//uniform vec3  albedo;
	//uniform float roughness;
	//uniform float ao;
	//uniform float metallic;
	glUniform3fv(sphereColorLocation, 1, &sphereColor.x);		// albedo
	glUniform1f(roughnessLocation, roughness);					// roughness
	glUniform1f(aoLocation, ambientOcclusion);					// ambient occlusion
	glUniform1f(metallicLocation, metallic);					// metallic

	// lights
	//uniform vec3 lightPositions[4];
	//uniform vec3 lightColors[4];
	glUniform3fv(lightPosLocation, 1, &lightPos[0].x);
	glUniform3fv(lightColLocation, 1, &lightCol[0].x);

	//uniform vec3 camPos;
	vec3<float> camPos = camera.GetEyePosition();
	glUniform3fv(camPosLocation, 1, &camPos.x);

	sphere->Draw();

	Draw::FinishDrawing();
}

void PBRDemo::ResetCamera()
{

}

void PBRDemo::HandleResizeEvent(const int& new_width, const int& new_height)
{
	new_width;
	new_height;
}

void PBRDemo::HandleKeyPress(KeyboardButton button)
{
	button;
}

void PBRDemo::HandleKeyRelease(KeyboardButton button)
{
	button;
}

void PBRDemo::HandleScrollEvent(float scroll_amount)
{
	scroll_amount;
}

void PBRDemo::HandleFocusEvent(bool focused)
{
	focused;
}
