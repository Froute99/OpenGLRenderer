/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *
 *	File_name: ShapeDrawingDemo.h
 *
 *	Simple Shape Drawing Demo
 *
 *	Dec.11 2024
 *******************************************************/

#pragma once
#include "Demo.hpp"
#include <Graphics/Shader.h>

class Object;
class GameObject;
struct aiScene;

class ShapeDrawingDemo : public Demo
{
public:
	explicit ShapeDrawingDemo(OpenGLWindow& window)
		: Demo(window) { Initialize(); }

	void Initialize() override final;
	void Update(float dt) override final;

	void ResetCamera() override final;

	void HandleResizeEvent(const int& new_width, const int& new_height) override final;
	void HandleKeyPress(KeyboardButton button) override final;
	void HandleKeyRelease(KeyboardButton button) override final;
	void HandleScrollEvent(float scroll_amount) override final;
	void HandleFocusEvent(bool focused) override final;

	void ImguiHelper();

private:
	vec3<float> moveSpeed;

	//Object* cube;
	GameObject* backpack;
	Object* skybox;

	Shader shader;
	Shader skyboxShader;

	GLuint uniformModelLocation;
	GLuint uniformViewLocation;
	GLuint uniformProjectionLocation;

	GLuint uniformObjectColorLocation;
	GLuint uniformLightPosLocation;
	GLuint uniformLightColorLocation;

	GLuint uniformLightCubeModel;
	GLuint uniformLightCubeView;
	GLuint uniformLightCubeProjection;

	vec3<float> objectColor;
	vec3<float> lightPos;
	vec3<float> lightColor;

	// imgui
	vec3<float> backpackTranslation;
	vec3<float> backpackRotationOffset;
	vec3<float> backpackRotationSpeed{ 0.f, 1.f, 0.f };

	bool showObjectWindow = true;
	bool showLightWindow = true;

	// HDR
	Shader		 hdrShader;
	unsigned int hdrFBO;	  // framebuffer object handle for HDR
	unsigned int colorBuffer; // colorbuffer for HDR
	bool		 hdr = true;
	float		 exposure = 1.0f;

	void		 RenderQuad();
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
};
