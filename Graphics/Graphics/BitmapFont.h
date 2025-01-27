/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: BitmapFont.hpp
 *	
 *	BitmapFont that includes the font and character data
 *	
 *	Fall 2019
 *	Nov.23 2019
 */

#pragma once
#include "Texture.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>

class [[nodiscard]]BitmapFont
{
public:
	struct [[nodiscard]]character
	{
		wchar_t id = wchar_t(-1);
		unsigned short x = 0;
		unsigned short y = 0;
		unsigned short width = 0;
		unsigned short height = 0;
		short xOffset = 0;
		short yOffset = 0;
		short xAdvance = 0;
		unsigned char page = 0;
	};

	struct [[nodiscard]]information
	{
		short fontSize = 0;
		std::wstring fontName;
		unsigned short lineHeight = 0;
		unsigned short imageWidth = 0;
		unsigned short imageHeight = 0;
		unsigned short pagesCount = 0;
		std::vector<std::wstring> pageNames;
	};

	bool LoadFromFile(const std::filesystem::path& filename) noexcept;
	const information& GetInformation() const noexcept;
	character GetCharacter(wchar_t character_id) const noexcept;
	const Texture& GetTexture(int page_index) const noexcept;
	bool HasCharacter(wchar_t character_id) const noexcept;

private:
	bool CanParseFile(const std::filesystem::path& filename) noexcept;
	bool CanParseStream(std::wistream& stream) noexcept;

private:
	information                            details;
	std::unordered_map<wchar_t, character> characters;
	std::vector<Texture>                   pageTextures;
};
