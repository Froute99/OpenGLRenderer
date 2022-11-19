/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Transform.hpp
 *
 *	Transform
 *
 *	Fall 2019
 *	Oct.10 2019
 */

#pragma once
#include "vec2.hpp"
#include "mat3.hpp"

class [[nodiscard]] Transform
{
public:
	Transform() noexcept = default;
	mat3<float> GetModelToWorld() noexcept;
	mat3<float> GetWorldToModel() noexcept;

	float CalculateWorldDepth() const noexcept;

	float GetDepth() const noexcept { return depth; }
	void SetDepth(float new_depth) noexcept { depth = new_depth; }

	vec2<float> GetTranslation() const noexcept { return translation; }
	void SetTranslation(vec2<float> new_translation) noexcept { translation = new_translation; }

	vec2<float> GetScale() const noexcept { return scale; }
	void SetScale(vec2<float> new_scale) noexcept { scale = new_scale; }

	float GetRotation() const noexcept { return rotation; }
	void SetRotation(float new_rotation) noexcept { rotation = new_rotation; }

	const Transform* GetParent() const noexcept { return parent; }
	void SetParent(Transform * transform_parent) noexcept { parent = transform_parent; }

private:
	vec2<float> translation{ 0.0f };
	vec2<float> scale{ 1.0f };
	float rotation = 0.0f;
	float depth = 0.0f;
	Transform* parent = nullptr;
};