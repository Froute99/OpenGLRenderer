/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Draw.hpp
 *	
 *	Drawing functions
 *	
 *	Fall 2019
 *	Nov.02 2019
 */

#pragma once
#include "Shader.h"
#include "Vertices.h"
#include "Texture.hpp"
#include "Text.hpp"

struct Material
{
	Shader shader;
	Vertices vertices;
	mat3<float> ndc;
	Texture texture;
};

class Draw
{
public:
	static void StartDrawing();
	static void StartDrawing(const Color4f& background_color);
	static void FinishDrawing();
	static void draw(const Material& material);
	static void DrawShape(const Material& material);
	static void DrawText(const Shader& shader, const mat3<float>& ndc, const Text& text);
private:
};

