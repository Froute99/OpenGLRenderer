/*
 *	Author: JeongHak Kim
 *	File_name: RenderQueue.h
 *
 *	Render queue for batch drawing
 *  Including Render command which represents state(shader or material etc.) of the object to draw
 *
 *	2026. 03. 04
 */

#pragma once
#include <Math/mat4.hpp>
#include <vector>

class Shader;
class SimpleMaterialPBR;
class VertexObject;

struct RenderCommand
{
	RenderCommand(Shader* shader, VertexObject* vo, const mat4<float>& modelMatrix, SimpleMaterialPBR* m)
		: shader(shader)
		, vo(vo)
		, modelMatrix(modelMatrix)
		, material(m)
	{}
	Shader*				shader;
	VertexObject*		vo;
	mat4<float>			modelMatrix;
	SimpleMaterialPBR*	material;
};

class RenderQueue
{
public:
	void Push(const RenderCommand& cmd) { commands.push_back(cmd); }

	void Sort() noexcept;

	void Draw() noexcept;

	void Clear() noexcept { commands.clear(); }

private:
	std::vector<RenderCommand> commands;

};
