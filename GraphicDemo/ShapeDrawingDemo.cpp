/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *
 *	File_name: ShapeDrawingDemo.cpp
 *
 *	Simple Shape Drawing Demo
 *
 *	Nov.25 2019
 *******************************************************/

#include <iostream>
#include "GL/glew.h"
#include "glfw3.h"
#include "ShapeDrawingDemo.hpp"
#include "Draw.hpp"
#include "PATH.hpp"
#include "Screenshot.hpp"
#include "GameObject.h"
#include <Angle.hpp>
#include <vec3.hpp>
#include "Mesh3D.h"

void ShapeDrawingDemo::Initialize()
{
	shader.LoadShaderFrom(PATH::shape_vert, PATH::shape_frag);
	lightCubeShader.LoadShaderFrom(PATH::lightCubeVS, PATH::lightCubeFS);
	layout = { VerticesDescription::Type::Position, VerticesDescription::Type::Normal };

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	Shader::UseShader(shader);
	objectColor = { 1.0f, 0.5f, 0.31f };
	const Mesh3D cube = MESH::BuildCube({ 0.f, 0.f, 0.f }, 0.5f, objectColor);
	vertices.InitializeWithMeshAndLayout(cube, layout);

	Shader::UseShader(lightCubeShader);
	lightPos = { 0.7f, 0.4f, 0.8f };
	lightColor = { 1.0f, 1.0f, 1.0f };
	const Mesh3D lightCube = MESH::BuildCube({ 0.f, 0.f, 0.f }, 0.2f, lightColor);
	lightCubeVertices.InitializeWithMeshAndLayout(lightCube, layout);

	uniformModelLocation = glGetUniformLocation(shader.GetHandleToShader(), "model");
	uniformViewLocation = glGetUniformLocation(shader.GetHandleToShader(), "view");
	uniformProjectionLocation = glGetUniformLocation(shader.GetHandleToShader(), "projection");

	uniformObjectColorLocation = glGetUniformLocation(shader.GetHandleToShader(), "objectColor");
	uniformLightPosLocation = glGetUniformLocation(shader.GetHandleToShader(), "lightPos");
	uniformLightColorLocation = glGetUniformLocation(shader.GetHandleToShader(), "lightColor");

	uniformLightCubeModel = glGetUniformLocation(lightCubeShader.GetHandleToShader(), "model");
	uniformLightCubeView = glGetUniformLocation(lightCubeShader.GetHandleToShader(), "view");
	uniformLightCubeProjection = glGetUniformLocation(lightCubeShader.GetHandleToShader(), "projection");


	// vec2<float> size{ 50.0f };
	// const Mesh rectangleMesh = MESH::create_rectangle({ 0.0f }, size, color);
	// rectangle = new GameObject(rectangleMesh, layout);
	// rectangle->SetShader(&shader);
	////rectangleVertices.InitializeWithMeshAndLayout(rectangle, layout);

	// const Mesh lineMesh = MESH::create_line({ 100.0f, 0.0f }, { 50.0f, 50.0f }, { -50.0f, -50.0f }, color);
	// line = new GameObject(lineMesh, layout);
	// line->SetShader(&shader);
	////lineVertices.InitializeWithMeshAndLayout(line, layout);

	// const Mesh quadMesh = MESH::create_quad({ 0.0f, 100.0f }, { -20.f, 25.f }, { -50.f, -25.f }, { 50.f, -25.f }, { 20.f, 25.f }, color);
	// quad = new GameObject(quadMesh, layout);
	// quad->SetShader(&shader);
	////quadVertices.InitializeWithMeshAndLayout(quad, layout);

	// const Mesh triangleMesh = MESH::create_triangle({ 100.0f }, { 0.0f, 50.0f }, { -30.f, 0.0f }, { 30.f, 0.0f }, color);
	// triangle = new GameObject(triangleMesh, layout);
	// triangle->SetShader(&shader);
	////triangleVertices.InitializeWithMeshAndLayout(triangle, layout);

	// const Mesh circleMesh = MESH::create_ellipse({ 200.0f, 25.0f }, size, 30, color);
	// circle = new GameObject(circleMesh, layout);
	// circle->SetShader(&shader);
	////circleVertices.InitializeWithMeshAndLayout(circle, layout);

	// const Mesh ellipseMesh = MESH::create_ellipse({ -200.0f, 25.0f }, { 50.0f, 30.f }, 30, color);
	// ellipse = new GameObject(ellipseMesh, layout);
	// ellipse->SetShader(&shader);
	////ellipseVertices.InitializeWithMeshAndLayout(ellipse, layout);

	// view.SetViewSize(width, height);
	// std::cout << "\t====================================\n";
	// std::cout << "\tPress Arrow keys to move the camera\n";
	// std::cout << "\tPress Z, X keys to rotate the camera\n";
	// std::cout << "\tPress \"Enter\" key to screenshot\n";
	// std::cout << "\t====================================\n";

	// const mat3<float> cameraToNDC = view.GetCameraToNDCTransform() * camera.WorldToCamera();

	// rectangle->SetNDC(cameraToNDC);
	// line->SetNDC(cameraToNDC);
	// quad->SetNDC(cameraToNDC);
	// triangle->SetNDC(cameraToNDC);
	// circle->SetNDC(cameraToNDC);
	// ellipse->SetNDC(cameraToNDC);
}

void ShapeDrawingDemo::Update(float dt)
{
	dt;
	// std::cout << "\r" << dt;

	if (!isFocused)
	{
		Draw::StartDrawing();
		return;
	}

	// camera.Rotate(rotationSpeed);
	// camera.MoveRight(moveSpeed.x);
	// camera.MoveUp(moveSpeed.y);

	Draw::StartDrawing();

	// transform.SetRotation(angle, 0, 0);
	angle += 0.005f;
	std::cout << "\r"
			  << "angle - " << angle;

	mat4<float> Model;

	mat4<float> Translation(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 3.0f, 1.0f);

	mat4<float> Rotation(
		cosf(angle), 0.0f, -sinf(angle), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinf(angle), 0.0f, cosf(angle), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	Model = Translation * Rotation;

	mat4<float> View = camera.BuildViewMatrix();

	mat4<float> Projection = view.BuildProjectionMatrix();
	// column-major operations should be ordered like v2 = PVM * v1;

	Shader::UseShader(shader);
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, &Model.elements[0][0]);
	glUniformMatrix4fv(uniformViewLocation, 1, GL_FALSE, &View.elements[0][0]);
	glUniformMatrix4fv(uniformProjectionLocation, 1, GL_FALSE, &Projection.elements[0][0]);

	glUniform3fv(uniformObjectColorLocation, 1, &objectColor.x);
	glUniform3fv(uniformLightPosLocation, 1, &lightPos.x);
	glUniform3fv(uniformLightColorLocation, 1, &lightColor.x);

	VertexObject::SelectVAO(vertices);
	glDrawArrays(vertices.GetPattern(), 0, vertices.GetVerticesCount());

	Model = mat4<float>(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		lightPos.x, lightPos.y, lightPos.z, 1.0f);

	Shader::UseShader(lightCubeShader);
	glUniformMatrix4fv(uniformLightCubeModel, 1, GL_FALSE, &Model.elements[0][0]);
	glUniformMatrix4fv(uniformLightCubeView, 1, GL_FALSE, &View.elements[0][0]);
	glUniformMatrix4fv(uniformLightCubeProjection, 1, GL_FALSE, &Projection.elements[0][0]);

	VertexObject::SelectVAO(lightCubeVertices);
	glDrawArrays(lightCubeVertices.GetPattern(), 0, lightCubeVertices.GetVerticesCount());


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
