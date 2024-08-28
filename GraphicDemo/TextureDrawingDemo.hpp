/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	
 *	File_name: TextureDrawingDemo.hpp
 *	
 *	Simple Texture Drawing Demo
 *	
 *	Nov.29 2019
 *******************************************************/

#pragma once
#include "Demo.hpp"
#include "Shader.h"
#include "Mesh.h"
#include "Vertices.h"
#include "VerticesDescription.h"
#include "Texture.hpp"
#include "Transform.hpp"
#include "Animation.hpp"
#include "BitmapFont.hpp"
#include "Text.hpp"
#include "Draw.hpp"

class TextureDrawingDemo : public Demo
{
public:
	explicit TextureDrawingDemo(OpenGLWindow& window) : Demo(window) { Initialize(); }
	void Initialize() override final;
	void Update(float dt) override final;

	void ResetCamera() override final;

	void HandleKeyPress(KeyboardButton button) override final;
	void HandleResizeEvent(const int& new_width, const int& new_height) override final;
	void HandleScrollEvent(float scroll_amount) override final;
	void HandleFocusEvent(bool focused) override final;
private:
	const std::filesystem::path& dragon_png = "../assets/dragon.png";
	const std::filesystem::path& blue_fire_sprite_sheet = "../assets/blue_fire.png";
private:
	float timePassed;
	mat4<float> cameraToNDC;
	VerticesDescription layout;
	Shader shader;

	float fireDistance = 300.0f;

	GameObject* dragon;

	//Material dragon;
	//Transform dragonTransform;

	//Material blueFire;
	//Animation blueFireAnimation;
	//Transform blueFireTransform;

	//BitmapFont font;
	//Text text;
	//Transform textTransform;
};
