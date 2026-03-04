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
#include "Material.h"

class Shader;

struct RenderCommand
{
	RenderCommand(Shader* shader, SimpleMaterialPBR* material, int handleToVAO, const mat4<float>& modelMatrix)
		: shader(shader), material(material), handleToVAO(handleToVAO), modelMatrix(modelMatrix)
	{}
	Shader*				shader;
	SimpleMaterialPBR*	material;
	int					handleToVAO;
	mat4<float>			modelMatrix;
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
