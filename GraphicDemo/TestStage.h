/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *
 *	File_name: PhongShadingDemo.h
 *
 *	Simple Shape Drawing Demo
 *
 *	Dec.11 2024
 *******************************************************/

#pragma once
#include "Demo.h"
#include <Graphics/Shader.h>
#include <Graphics/IBL.h>

class Object;
class GameObject;
struct aiScene;

class TestStage : public Demo
{
public:
	explicit TestStage(OpenGLWindow& window)
		: Demo(window) { Initialize(); }

	void Initialize() override final;
	void Update(float dt) override final;

	void ResetCamera() override final;

	void HandleResizeEvent(const int new_width, const int new_height) override final;
	void HandleKeyPress(KeyboardButton button) override final;
	void HandleKeyRelease(KeyboardButton button) override final;
	void HandleScrollEvent(float scroll_amount) override final;
	void HandleFocusEvent(bool focused) override final;

	void ImguiHelper();

	void renderCube();
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;

private:
	vec3<float> moveSpeed;

	GameObject* cube;
	EnvironmentMap envMap{ GetScreenWidth(), GetScreenHeight() };

	Shader shader;
	Shader skyboxShader;
	Shader testShader;

	unsigned int hdrTexture = 0;
	unsigned int envCubemap = 0;

	// IMGui
	vec3<float> objectColor;
	vec3<float> lightPos;
	vec3<float> lightColor;

	bool showObjectWindow = true;
	bool showLightWindow = true;
};
