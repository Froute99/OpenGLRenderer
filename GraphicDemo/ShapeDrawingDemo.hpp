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

	mat3<float> ndc;
	mat3<float> lineNDC;
	mat3<float> quadNDC;
	mat3<float> ellipseNDC;
	mat3<float> triangleNDC;
	mat3<float> circleNDC;
	
	VerticesDescription layout{ VerticesDescription::Type::Point, VerticesDescription::Type::Color };
	Shader shader;

	Mesh rectangle;
	Vertices rectangleVertices;
	Transform rectangleTransform;
	
	Mesh line;
	Vertices lineVertices;
	Transform lineTransform;

	Mesh quad;
	Vertices quadVertices;
	Transform quadTransform;

	Mesh triangle;
	Vertices triangleVertices;
	Transform triangleTransform;

	Mesh circle;
	Vertices circleVertices;
	Transform circleTransform;

	Mesh ellipse;
	Vertices ellipseVertices;
	Transform ellipseTransform;
};
