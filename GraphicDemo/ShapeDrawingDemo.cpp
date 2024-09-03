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

//#define GLM_ENABLE_EXPERIMENTAL
//#include "glm.hpp"
//#include "detail/type_mat4x4.hpp"
//#include "gtx/projection.hpp"
//#include "gtx/matrix_operation.hpp"
//#include "gtc/matrix_transform.hpp"

struct Vertex
{
	vec3<float> pos;
	vec3<float> color;

	Vertex() {}

	Vertex(float x, float y, float z)
	{
		pos = vec3<float>(x, y, z);

		float red = (float)rand() / (float)RAND_MAX;
		float green = (float)rand() / (float)RAND_MAX;
		float blue = (float)rand() / (float)RAND_MAX;
		color = vec3<float>(red, green, blue);
	}
};

void ShapeDrawingDemo::CreateVertexBuffer()
{
	Vertex vertices[8];

	vertices[0] = { 0.5f, 0.5f, 0.5f };
	vertices[1] = { -0.5f, 0.5f, -0.5f };
	vertices[2] = { -0.5f, 0.5f, 0.5f };
	vertices[3] = { 0.5f, -0.5f, -0.5f };
	vertices[4] = { -0.5f, -0.5f, -0.5f };
	vertices[5] = { 0.5f, 0.5f, -0.5f };
	vertices[6] = { 0.5f, -0.5f, 0.5f };
	vertices[7] = { -0.5f, -0.5f, 0.5f };

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void ShapeDrawingDemo::CreateIndexBuffer()
{
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(float), Indices, GL_STATIC_DRAW);
}

void ShapeDrawingDemo::Initialize()
{
	shader.LoadShaderFrom(PATH::shape_vert, PATH::shape_frag);
	glUseProgram(shader.GetHandleToShader());
	layout = { VerticesDescription::Type::Point, VerticesDescription::Type::Color };

	const Color4f color{ .8f, .8f, .0f, 1.0f };

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	CreateVertexBuffer();
	CreateIndexBuffer();

	uniformNDClocation = glGetUniformLocation(shader.GetHandleToShader(), "toNDC");

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
		0.0f, 0.0f, 2.5f, 1.0f);

	 mat4<float> Rotation(
		 cosf(angle), 0.0f, -sinf(angle), 0.0f,
		 0.0f, 1.0f, 0.0f, 0.0f,
		 sinf(angle), 0.0f, cosf(angle), 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f);

	 Model = Translation * Rotation;

	 float FOV = 90.0f;
	 float tanHalfFOV = tanf(ANGLE::DegreeToRadian(FOV / 2.0f));
	 float near = 0.1f;

	 //mat4<float> View = Matrix4::build_identity<float>();
	 mat4<float> View = camera.LookAt();

	 mat4<float> Projection(
		1.0f / tanHalfFOV, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f / tanHalfFOV, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, -2.f * near, 0.0f);


	// mat4<float> FinalMatrix = Rotation * Translation * Projection;		// this means row-major matrix multiplication
	//  column-major operations should be ordered like v2 = PVM * v1;
	 mat4<float> FinalMatrix = (Projection) * (View) * (Translation) * (Rotation);

	//glm::mat4 Translation(1);
	//glm::mat4 Rotation(1);

	//// translate
	//Translation = glm::translate(Translation, glm::vec3(0.f, 0.f, 5.f));

	//// rotate
	//Rotation = glm::rotate(Rotation, angle, glm::vec3(0.f, 1.f, 0.f));

	//glm::vec3 eye = { 0.f, 0.f, 0.f };
	//glm::vec3 center = { 0.f, 0.f, 1.f };
	//glm::vec3 up = { 0.f, 1.f, 0.f };

	//glm::mat4 View = glm::lookAtLH(eye, center, up);

	//glm::mat4 Projection = glm::infinitePerspectiveLH_NO(glm::radians(90.0f), 1.0f, 0.1f);

	//glm::mat4 FinalMatrix = Projection * View * Translation * Rotation;
	GLuint	  uniformHandle = glGetUniformLocation(shader.GetHandleToShader(), "gWorld");
	glUniformMatrix4fv(uniformHandle, 1, GL_FALSE, &FinalMatrix.elements[0][0]);
	if (uniformHandle == -1)
	{
		printf("Error getting uniform location of 'gWorld'\n");
		exit(1);
	}

	glUseProgram(shader.GetHandleToShader());

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// Draw::DrawShape(*rectangle->GetMaterial());
	// Draw::DrawShape(*line->GetMaterial());
	// Draw::DrawShape(*quad->GetMaterial());
	// Draw::DrawShape(*triangle->GetMaterial());
	// Draw::DrawShape(*circle->GetMaterial());
	// Draw::DrawShape(*ellipse->GetMaterial());

	// Draw::DrawGameObject(DrawType::Shape, rectangle->GetMaterial());
	// Draw::DrawGameObject(DrawType::Shape, line->GetMaterial());
	// Draw::DrawGameObject(DrawType::Shape, quad->GetMaterial());
	// Draw::DrawGameObject(DrawType::Shape, triangle->GetMaterial());
	// Draw::DrawGameObject(DrawType::Shape, circle->GetMaterial());
	// Draw::DrawGameObject(DrawType::Shape, ellipse->GetMaterial());

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
