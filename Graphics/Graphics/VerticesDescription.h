/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: VerticesDescription.h
 *	
 *	VerticesDecription class for describe information of vertices
 *	
 *	Fall 2019
 *	Oct.31 2019
 */

#pragma once
#include <vector>

class [[nodiscard]] VerticesDescription
{
public:
	enum class Type
	{
		Position,
		Normal,
		Color,
		TextureCoordinate
	};

	struct TypeDescription
	{
		unsigned int elementsNumber = 0;
		unsigned int elementsType = 0;
		unsigned int sizeInBytes = 0;
		bool shouldNormalize = false;
	};

	void AddType(Type type) noexcept;
	const std::vector<Type>& GetTypes() const noexcept;

	VerticesDescription() = default;
	VerticesDescription(std::initializer_list<Type> fields) noexcept;

	unsigned GetVertexSize() const noexcept;

	void EnableAttributes() const noexcept;
	
private:
	unsigned vertexSize = 0;
	std::vector<TypeDescription> typeDescription{};
	std::vector<Type> types{};
};
