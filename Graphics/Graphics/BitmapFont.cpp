/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: BitmapFont.cpp
 *	
 *	BitmapFont that includes the font and character data
 *	
 *	Fall 2019
 *	Nov.23 2019
 */

#include <filesystem>
#include <fstream>
#include <sstream>
#include <cassert>
#include "BitmapFont.h"
#include "Texture.h"

bool BitmapFont::LoadFromFile(const std::filesystem::path& filename) noexcept
{
	if (!CanParseFile(filename))
	{
		return false;
	}
	for (int i = 0; i < details.pagesCount; ++i)
	{
		Texture input;
		std::wstring inputPath = L"../assets/";
		details.pageNames.at(i).pop_back();		// last "
		details.pageNames.at(i) = details.pageNames.at(i).substr(1);	// first "
		inputPath.append(details.pageNames.at(i));
		input.LoadFromPath(inputPath);
		pageTextures.push_back(std::move(input));
	}
	return true;
}

const BitmapFont::information& BitmapFont::GetInformation() const noexcept
{
	return details;
}

BitmapFont::character BitmapFont::GetCharacter(wchar_t character_id) const noexcept
{
	if (HasCharacter(character_id))
	{
		return characters.find(character_id)->second;
	}
	return character();
}

const Texture& BitmapFont::GetTexture(int page_index) const noexcept
{
	assert(!pageTextures.empty() == true);
	assert(0 <= page_index && page_index < details.pagesCount);
	return pageTextures.at(page_index);
}

bool BitmapFont::HasCharacter(wchar_t character_id) const noexcept
{
	return !(characters.find(character_id) == characters.end());
}

bool BitmapFont::CanParseFile(const std::filesystem::path& filename) noexcept
{
	std::wifstream inputFile(filename);
	return CanParseStream(inputFile);
}

bool BitmapFont::CanParseStream(std::wistream& stream) noexcept
{
	std::wstring line, read, key, value;
	std::size_t i;
	while (!stream.eof())
	{
		std::wstringstream LineStream;
		std::getline(stream, line);
		LineStream << line;

		LineStream >> read;
		if (read == L"info")
		{
			while (!LineStream.eof())
			{
				std::wstringstream Converter;
				LineStream >> read;
				i = read.find('=');
				key = read.substr(0, i);
				value = read.substr(i + 1);

				Converter << value;
				if (key == L"face")
				{
					std::size_t count1 = line.find('\"');
					std::size_t count2 = line.find('\"', count1 + 1);
					std::wstring name;
					name = line.substr(count1, count2 - count1);
					details.fontName = name;
				}
				else if (key == L"size")
				{
					Converter >> details.fontSize;
				}
			}
		}
		else if (read == L"common")
		{
			while (!LineStream.eof())
			{
				std::wstringstream Converter;
				LineStream >> read;
				i = read.find('=');
				key = read.substr(0, i);
				value = read.substr(i + 1);

				Converter << value;
				if (key == L"lineHeight")
				{
					Converter >> details.lineHeight;
				}
				else if (key == L"scaleW")
				{
					Converter >> details.imageWidth;
				}
				else if (key == L"scaleH")
				{
					Converter >> details.imageHeight;
				}
				else if (key == L"pages")
				{
					Converter >> details.pagesCount;
				}
			}
		}
		else if (read == L"page")
		{
			while (!LineStream.eof())
			{
				LineStream >> read;
				i = read.find('=');
				key = read.substr(0, i);
				value = read.substr(i + 1);

				if (key == L"file")
				{
					details.pageNames.push_back(value);
				}
			}
		}
		else if (read == L"char")
		{
			character temp;
			while (!LineStream.eof())
			{
				std::wstringstream Converter;
				LineStream >> read;
				i = read.find('=');
				key = read.substr(0, i);
				value = read.substr(i + 1);

				Converter << value;
				if (key == L"id")
				{
					temp.id = wchar_t(std::stoi(value));
				}
				else if (key == L"x")
				{
					Converter >> temp.x;
				}
				else if (key == L"y")
				{
					Converter >> temp.y;
				}
				else if (key == L"width")
				{
					Converter >> temp.width;
				}
				else if (key == L"height")
				{
					Converter >> temp.height;
				}
				else if (key == L"xoffset")
				{
					Converter >> temp.xOffset;
				}
				else if (key == L"yoffset")
				{
					Converter >> temp.yOffset;
				}
				else if (key == L"xadvance")
				{
					Converter >> temp.xAdvance;
				}
				else if (key == L"page")
				{
					short page;
					Converter >> page;
					temp.page = static_cast<unsigned char>(page);
				}
			}
			characters.insert_or_assign(temp.id, temp);
		}
	}
	return true;
}
