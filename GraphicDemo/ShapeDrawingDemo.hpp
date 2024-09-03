/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	
 *	File_name: ShapeDrawingDemo.h
 *	
 *	Simple Shape Drawing Demo
 *	
 *	Nov.25 2019
 *******************************************************/

#pragma once
#include "Demo.hpp"
#include "Shader.h"
#include "Mesh.h"
#include "Vertices.h"
#include "VerticesDescription.h"
#include "Texture.hpp"
#include "Transform.hpp"

class GameObject;

class ShapeDrawingDemo : public Demo
{
public:
	explicit ShapeDrawingDemo(OpenGLWindow& window) : Demo(window) { Initialize(); }

	void Initialize() override final;
	void Update(float dt) override final;

	void ResetCamera() override final;

	void HandleResizeEvent(const int& new_width, const int& new_height) override final;
	void HandleKeyPress(KeyboardButton button) override final;
	void HandleKeyRelease(KeyboardButton button) override final;
	void HandleScrollEvent(float scroll_amount) override final;
	void HandleFocusEvent(bool focused) override final;
private:
	vec2<float> moveSpeed{ 0.f };
	float rotationSpeed = 0;
	
	VerticesDescription layout;
	Shader shader;

	//GameObject* rectangle;
	//GameObject* line;
	//GameObject* quad;
	//GameObject* triangle;
	//GameObject* circle;
	//GameObject* ellipse;

	GLuint VBO;
	GLuint IBO;

	unsigned int Indices[36] = {
		0, 1, 2,
		1, 3, 4,
		5, 6, 3,
		7, 3, 6,
		2, 4, 7,
		0, 7, 6,
		0, 5, 1,
		1, 5, 3,
		5, 0, 6,
		7, 4, 3,
		2, 1, 4,
		0, 2, 7
	};

	void CreateVertexBuffer();
	void CreateIndexBuffer();

	GLuint uniformNDClocation;

	float angle = 0;

	//Mesh rectangle;
	//Vertices rectangleVertices;
	//Transform rectangleTransform;

	//Mesh line;
	//Vertices lineVertices;
	//Transform lineTransform;

	//Mesh quad;
	//Vertices quadVertices;
	//Transform quadTransform;

	//Mesh triangle;
	//Vertices triangleVertices;
	//Transform triangleTransform;

	//Mesh circle;
	//Vertices circleVertices;
	//Transform circleTransform;

	//Mesh ellipse;
	//Vertices ellipseVertices;
	//Transform ellipseTransform;
};
