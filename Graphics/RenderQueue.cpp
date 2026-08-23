/*
 *	Author: JeongHak Kim
 *	File_name: RenderQueue.cpp
 *
 *	Render queue for batch drawing
 *
 *	2026. 03. 04
 */

#include "RenderQueue.h"
#include "Material.h"
#include "Graphics/Shader.h"
#include "Graphics/Vertices.h"
#include <glew.h>
#include <algorithm>
#include "Graphics/Draw.h"

auto compare = [](const RenderCommand& a, const RenderCommand& b) {
	if (a.shader != b.shader)
		return a.shader < b.shader;
	if (a.material != b.material)
		return a.material < b.material;
	return false;		// because in my case of optimization, there's only two case above.
};

void RenderQueue::Sort() noexcept
{
	std::sort(commands.begin(), commands.end(), compare);
}

void RenderQueue::Draw() noexcept
{
	// Assume RenderQueue is guaranteed to be sorted
	Shader*			   currentShader = nullptr;
	SimpleMaterialPBR* currentMaterial = nullptr;

	for (const auto& c : commands)
	{
		if (c.shader != currentShader)
		{
			currentShader = c.shader; 
			currentShader->Use();
		}

		if (c.material != currentMaterial)
		{
			currentMaterial = c.material;
			currentMaterial->ApplyToShader(currentShader);
		}

		// TODO: check VAO handle caching; objects using the same mesh


		// TODO: might need a new-fancy Render class
		currentShader->SendUniformVariable("model", c.modelMatrix);

		Draw::DrawElementsWithCount(c.vo->VAO, c.vo->GetPattern(), c.vo->GetIndicesCount());
		//glBindVertexArray(c.vo->VAO);
		//glDrawElements(c.vo->GetPattern(), c.vo->GetIndicesCount(), GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);
	}
}
