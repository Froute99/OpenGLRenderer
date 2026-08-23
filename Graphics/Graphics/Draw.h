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
#include "Texture.h"
#include "Text.h"
#include <Math/mat4.hpp>

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
	//static void StartDrawing();
	static void StartDrawing(const Color4f& background_color = { 0.f });
	static void FinishDrawing();
	static void DrawSprite(const Material& material);
	static void DrawShape(const Material& material);
	static void DrawGameObject(DrawType type, Material* target);
	static void DrawText(const Shader& shader, const mat4<float>& ndc, const Text& text);
	static void DrawElements(unsigned int vao, unsigned int pattern, size_t indicesSize);
	static void DrawElementsWithCount(unsigned int, unsigned int, size_t);

	static int GetDrawCount() { return count; }

private:
	inline static int count;

};

