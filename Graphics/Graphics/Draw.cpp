/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Draw.cpp
 *	
 *	Drawing functions
 *	
 *	Fall 2019
 *	Nov.02 2019
 */

#include "Draw.h"
#include <glew.h>

void Draw::StartDrawing()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/* Background will be cleared with parameter */
void Draw::StartDrawing(const Color4f& background_color)
{
	glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Draw::FinishDrawing()
{
	glFinish();
}

void Draw::DrawSprite(const Material& material)
{
	glBindTexture(GL_TEXTURE_2D, material.texture.GetTexturehandle());
	Shader::UseShader(*material.shader);
	//material.shader->SendUniformVariable("ndc", material.ndc);
	VertexObject::SelectVAO(material.vertices);
	glDrawArrays(material.vertices.GetPattern(), 0, material.vertices.GetVerticesCount());
}

void Draw::DrawShape(const Material& material)
{
	Shader::UseShader(*material.shader);
	//material.shader->SendUniformVariable("ndc", material.ndc);
	VertexObject::SelectVAO(material.vertices);
	glDrawArrays(material.vertices.GetPattern(), 0, material.vertices.GetVerticesCount());
}

void Draw::DrawGameObject(DrawType type, Material* target)
{
	switch (type)
	{
	case DrawType::Shape:
		DrawShape(*target);
		break;
	case DrawType::Sprite:
		DrawSprite(*target);
		break;
	case DrawType::Text:
		//DrawText()
		break;
	default:
		break;
	}
}

//void Draw::DrawText(const Shader& shader, const mat3<float>& ndc, const Text& text)
//{
//	for (const auto& vertices_texture : text.GetPairOfVerticesAndTextures())
//	{
//		const VertexObject& textVertices = *vertices_texture.first;
//		const Texture*  textTexture  = vertices_texture.second;
//		DrawSprite({ shader, textVertices, ndc, *textTexture });
//	}
//}
