/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Text.cpp
 *	
 *	Text class that makes vertices and texture of characters
 *	
 *	Fall 2019
 *	Nov.23 2019
 */

#include "Text.hpp"

Text::Text(std::wstring text_string, const BitmapFont& text_font) noexcept
{
	string = text_string;
	font = &text_font;
	InitializeWithEmptyVertices();
}

std::vector<std::pair<const Vertices*, const Texture*>> Text::GetPairOfVerticesAndTextures() const noexcept
{
	std::vector<std::pair<const Vertices*, const Texture*>> pairs;

	BuildNewMeshesIfNeeded();
	for (int i = 0; i < font->GetInformation().pagesCount; ++i)
	{
		if (vertices.at(i).GetVerticesCount() != 0)
		{
			std::pair<const Vertices*, const Texture*> input;
			input.first = &vertices.at(i);
			input.second = &font->GetTexture(i);
			pairs.push_back(input);
		}
	}
	return pairs;
}

void Text::SetString(const std::wstring& text_string) noexcept
{
	if (text_string != string)
	{
		needNewMeshes = true;
		string = text_string;
	}
}

void Text::SetFont(const BitmapFont& text_font) noexcept
{
	font = &text_font;
	InitializeWithEmptyVertices();
	needNewMeshes = true;
}

void Text::InitializeWithEmptyVertices() const noexcept
{
	for (int i = 0; i < font->GetInformation().pagesCount; ++i)
	{
		Mesh meshToInput;
		Vertices verticesToInput;
		verticesToInput.InitializeWithMeshAndLayout(meshToInput,
			{ VerticesDescription::Type::Point, VerticesDescription::Type::TextureCoordinate });
		vertices.insert_or_assign(i, std::move(verticesToInput));
	}
}

void Text::BuildNewMeshesIfNeeded() const noexcept
{
	vec2<float> cursor(0, 0);
	if (needNewMeshes)
	{
		std::vector<Mesh> meshes;
		for (int i = 0; i < font->GetInformation().pagesCount; ++i)
		{
			Mesh mesh;
			meshes.push_back(mesh);
		}
		for (auto curChar : string)
		{
			BitmapFont::character ch = font->GetCharacter(curChar);
			BitmapFont::information chDesc = font->GetInformation();
			if (curChar == ' ')
			{
				if (font->HasCharacter(' '))
				{
					cursor.x += font->GetCharacter(' ').xAdvance;
				}
				else
				{
					cursor.x += chDesc.fontSize;
				}
			}
			else if (curChar == '\t')
			{
				if (font->HasCharacter(' '))
				{
					cursor.x += 4 * font->GetCharacter(' ').xAdvance;
				}
				else
				{
					cursor.x += 4 * chDesc.fontSize;
				}
			}
			else if (curChar == '\n')
			{
				cursor.y -= chDesc.lineHeight;
				cursor.x = 0;
			}
			else
			{
				float left = cursor.x + ch.xOffset;
				float right = left + ch.width;
				float bottom = -1 * (ch.yOffset + ch.height) + chDesc.lineHeight + cursor.y;
				float top = bottom + ch.height;

				float textureLeft = ch.x / float(chDesc.imageWidth);
				float textureTop = 1.0f - ch.y / float(chDesc.imageHeight);
				float textureRight = (ch.x + ch.width) / float(chDesc.imageWidth);
				float textureBottom = 1.0f - (ch.y + ch.height) / float(chDesc.imageHeight);

				meshes.at(ch.page).SetShapePattern(ShapePattern::Triangles);
				meshes.at(ch.page).AddPoint({ left, top });
				meshes.at(ch.page).AddPoint({ right, top });
				meshes.at(ch.page).AddPoint({ left, bottom });
				meshes.at(ch.page).AddTextureCoordinate({ textureLeft, textureTop });
				meshes.at(ch.page).AddTextureCoordinate({ textureRight, textureTop });
				meshes.at(ch.page).AddTextureCoordinate({ textureLeft, textureBottom });

				meshes.at(ch.page).AddPoint({ right, top });
				meshes.at(ch.page).AddPoint({ left, bottom });
				meshes.at(ch.page).AddPoint({ right, bottom });
				meshes.at(ch.page).AddTextureCoordinate({ textureRight, textureTop });
				meshes.at(ch.page).AddTextureCoordinate({ textureLeft, textureBottom });
				meshes.at(ch.page).AddTextureCoordinate({ textureRight, textureBottom });
				
				cursor.x += ch.xAdvance;
			}
		}
		for (int i = 0; i < font->GetInformation().pagesCount; i++)
		{
			vertices.find(i)->second.InitializeWithMeshAndLayout(meshes.at(i),
				{ VerticesDescription::Type::Point, VerticesDescription::Type::TextureCoordinate });
		}
	}
	needNewMeshes = false;
}
