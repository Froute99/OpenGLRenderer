/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	
 *	File_name: AnimationDemo.hpp
 *	
 *	Animation Displaying
 *	
 *	Dec.03 2019
 *******************************************************/

#pragma once
#include "Demo.hpp"
#include "Animation.hpp"
#include "Mesh.h"
#include "Vertices.h"
#include "VerticesDescription.h"
#include "Texture.hpp"
#include "Transform.hpp"
#include "Draw.hpp"

class AnimationDemo : public Demo
{
public:
	explicit AnimationDemo(OpenGLWindow& window) : Demo(window) { Initialize(); }
	void Initialize() override final;
	void Update(float dt) override final;

	void ResetCamera() override final;

	void HandleResizeEvent(const int& new_width, const int& new_height) override final;
	void HandleFocusEvent(bool focused) override;
private:
	mat3<float> cameraToNDC;
	VerticesDescription layout{ VerticesDescription::Type::Position, VerticesDescription::Type::TextureCoordinate };
	Shader shader;

	Material fire;
	Material blueFire;
	Transform fireTransform;
	Transform blueFireTransform;

	Animation fireAnimation;
	Animation blueFireAnimation;
};

