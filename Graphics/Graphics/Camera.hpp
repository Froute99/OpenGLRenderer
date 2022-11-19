/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Camera.hpp
 *
 *	Camera class
 *
 *	Fall 2019
 *	Oct.10 2019
 */

#pragma once
#include "vec2.hpp"
#include "mat3.hpp"

class [[nodiscard]]Camera
{
public:
	constexpr Camera() noexcept = default;
	Camera(vec2<float> camera_center, vec2<float> camera_up) noexcept;

	vec2<float> GetCenter() const noexcept { return center; }
	void    SetCenter(vec2<float> camera_center) noexcept { center = camera_center; }
	vec2<float> GetUp() const noexcept { return up; }
	vec2<float> GetRight() const noexcept { return right; }

	void ResetUp(vec2<float> camera_up = {0, 1}) noexcept;

	void MoveUp(float distance) noexcept;
	void MoveRight(float distance) noexcept;
	void Rotate(float angle_radians) noexcept;

	mat3<float> CameraToWorld() const noexcept;
	mat3<float> WorldToCamera() const noexcept;
	
	vec2<float> rotate_by(float angle_in_radians, vec2<float> v) noexcept
	{
		float cos_value = cos(angle_in_radians);
		float sin_value = sin(angle_in_radians);
		return vec2<float>{ cos_value * v.x - sin_value * v.y, sin_value* v.x + cos_value * v.y };
	}

private:
	vec2<float> center{};
	vec2<float> up{0.f , 1.f};
	vec2<float> right{1.f, 0.f};
};
