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
#include <mat4.hpp>

class GameObject;

struct Material
{
	Material() = default;
	Material(Shader* shader, VertexObject vertices, mat4<float> ndc, Texture texture)
		: shader(shader), vertices(vertices), ndc(ndc), texture(texture) {}

	Shader* shader;
	VertexObject vertices;
	mat4<float> ndc;
	Texture texture;
};

enum struct DrawType
{
	Shape,
	Sprite,
	Text
};

class Draw
{
public:
	static void StartDrawing();
	static void StartDrawing(const Color4f& background_color);
	static void FinishDrawing();
	static void DrawSprite(const Material& material);
	static void DrawShape(const Material& material);
	static void DrawGameObject(DrawType type, Material* target);
	static void DrawText(const Shader& shader, const mat4<float>& ndc, const Text& text);
private:
};

