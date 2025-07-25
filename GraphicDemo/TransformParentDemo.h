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
#include <Graphics/UBO.h>

class GameObject;
class Texture;

class TransformParentDemo : public Demo
{
public:
	explicit TransformParentDemo(OpenGLWindow& window) : Demo(window) { Initialize(); }

	void Initialize() override final;
	void Update(float dt) override final;

	void ResetCamera() override final;

	void HandleKeyPress(KeyboardButton button) override final;
	void HandleKeyRelease(KeyboardButton button) override final;
	void HandleResizeEvent(const int new_width, const int new_height) override final;
	void HandleMousePositionEvent(float xpos, float ypos) override final;
	void HandleMouseEvent(MouseButton button) override final;
	void HandleFocusEvent(bool focused) override final;

private:
	Shader shader;

	GameObject* earth;
	Texture*	earthAlbedo;

	GameObject* sun;
	Texture*	sunAlbedo;

	GameObject* moon;
	Texture*	moonAlbedo;

	vec3<float> lightPosition;
	vec3<float> lightColor;

	UBO matricesBlock{ 128 };

	//mat3<float> cameraToNDC;
};
