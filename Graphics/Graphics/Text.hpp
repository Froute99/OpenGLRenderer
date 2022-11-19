/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Text.cpp
 *	
 *	Text class that makes vertices and texture of characters
 *	
 *	Fall 2019
 *	Nov.23 2019
 */

#pragma once
#include <string>
#include <unordered_map>
#include "Mesh.h"
#include "Vertices.h"
#include "BitmapFont.hpp"

class BitmapFont;

class [[nodiscard]]Text
{
public:
	Text() = default;
	Text(std::wstring text_string, const BitmapFont & text_font) noexcept;

	std::vector<std::pair<const Vertices*, const Texture*>> GetPairOfVerticesAndTextures() const noexcept;

	void SetString(const std::wstring & text_string) noexcept;
	void SetFont(const BitmapFont & text_font) noexcept;

	std::wstring GetString() const noexcept { return string; };
	const BitmapFont* GetFont() const noexcept { return font; };
private:
	void InitializeWithEmptyVertices() const noexcept;
	void BuildNewMeshesIfNeeded() const noexcept;
private:
	mutable bool needNewMeshes = true;
	std::wstring string;
	const BitmapFont* font = nullptr;
	mutable std::unordered_map<int, Vertices> vertices;
};
