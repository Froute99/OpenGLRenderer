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
#include <Math/vec3.hpp>
#include <Math/mat4.hpp>

class [[nodiscard]] Camera
{
public:
	constexpr Camera() noexcept = default;
	Camera(vec3<float> newPos) noexcept;

	vec3<float> GetEyePosition() const noexcept { return eye; }
	void		SetEyePosition(vec3<float> newPos) noexcept { eye = newPos; }
	vec3<float> GetUp() const noexcept { return up; }
	vec3<float> GetRight() const noexcept { return right; }
	

	//void ResetUp(vec2<float> camera_up = { 0, 1 }) noexcept;

	//void MoveUp(float distance) noexcept;
	//void MoveRight(float distance) noexcept;
	//void Rotate(float angle_radians) noexcept;

	//mat4<float> CameraToWorld() const noexcept;
	//mat4<float> WorldToCamera() const noexcept;

	//vec3<float> rotate_by(float angle_in_radians, vec2<float> v) noexcept
	//{
	//	float cos_value = cos(angle_in_radians);
	//	float sin_value = sin(angle_in_radians);
	//	return vec3<float>{ cos_value * v.x - sin_value * v.y, sin_value * v.x + cos_value * v.y };
	//}

	mat4<float> BuildViewMatrix() const noexcept;

private:
	vec3<float> eye{ 0.0f, 0.0f, 0.0f };				// camera position
	vec3<float> target{ 0.0f, 0.0f, 1.0f };				// camera look at

	vec3<float> up{ 0.0f, 1.0f, 0.0f };
	vec3<float> right{ 1.0f, 0.0f, 0.0f };


	//vec3<float> up{ 0.f, 0.f, 1.f };
	//vec3<float> right{ 1.f, 0.f };
};
