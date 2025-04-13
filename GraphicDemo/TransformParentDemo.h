/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	
 *	File_name: TransformParentDemo.hpp
 *	
 *	Transform Set Parent Test
 *	
 *	Dec.07 2019
 *******************************************************/

#pragma once
#include "Demo.h"
#include <Graphics/Shader.h>
#include <Graphics/Mesh.h>
#include <Graphics/VerticesDescription.h>
#include <Graphics/Transform.h>
#include <Graphics/Draw.h>

class TransformParentDemo : public Demo
{
public:
	explicit TransformParentDemo(OpenGLWindow& window) : Demo(window) { Initialize(); }

	void Initialize() override final;
	void Update(float dt) override final;

	void ResetCamera() override final;

	void HandleKeyPress(KeyboardButton button) override final;
	void HandleResizeEvent(const int& new_width, const int& new_height) override final;
	void HandleMousePositionEvent(float xpos, float ypos) override final;
	void HandleMouseEvent(MouseButton button) override final;
	void HandleFocusEvent(bool focused) override final;
private:
	bool isClicked = false;
	float rotation;
	float rotationSpeed = 0.0f;
	mat3<float> cameraToNDC;
	VerticesDescription layout{ VerticesDescription::Type::Position, VerticesDescription::Type::TextureCoordinate };
	Shader shader;

	vec2<float> mousePosition;

	Material sword1;
	Transform sword1Transform;

	float slashRotation = 0.0f;
	Material slash1;
	Transform slash1Transform;

	Material slash2;
	Transform slash2Transform;
};

