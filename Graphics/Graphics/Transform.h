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
#include <Math/vec2.hpp>
#include <Math/mat4.hpp>

class [[nodiscard]] Transform
{
public:
	Transform() noexcept = default;
	//mat4<float> GetWorldToModel() noexcept;

	float CalculateWorldDepth() const noexcept;

	float GetDepth() const noexcept { return depth; }
	void SetDepth(float new_depth) noexcept { depth = new_depth; }

	vec3<float> GetTranslation() const noexcept { return translation; }
	void SetTranslation(vec3<float> new_translation) noexcept { translation = new_translation; }
	void SetTranslation(float x, float y, float z) noexcept { translation = { x,y,z }; }

	vec3<float> GetScale() const noexcept { return scale; }
	void SetScale(vec3<float> new_scale) noexcept { scale = new_scale; }
	void SetScale(float x, float y, float z) noexcept { scale = { x,y,z }; }

	vec3<float> GetRotation() const noexcept { return rotation; }
	float GetYaw() const noexcept { return rotation.x; }
	float GetPitch() const noexcept { return rotation.y; }
	float GetRoll() const noexcept { return rotation.z; }
	void SetRotation(vec3<float> new_rotation) noexcept { rotation = new_rotation; }
	void SetRotation(float x, float y, float z) noexcept { rotation = { x,y,z }; }

	const Transform* GetParent() const noexcept { return parent; }
	void SetParent(Transform * transform_parent) noexcept { parent = transform_parent; }

	mat4<float> BuildModelMatrix() const noexcept;

private:
	vec3<float> translation{ 0.0f };
	vec3<float> rotation{ 0.0f, 0.0f, 0.0f };	// yaw pitch roll	axis standard: Z Y X
	vec3<float> scale{ 1.0f };

	float depth = 0.0f;
	Transform* parent = nullptr;
};
