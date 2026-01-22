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
#include <Math/Angle.hpp>

struct GlobalUniformData;
class [[nodiscard]] Camera
{
public:
	constexpr Camera() noexcept = default;
	Camera(vec3<float> newPos) noexcept;
	~Camera() = default;

	vec3<float> GetEyePosition() const noexcept { return eye; }
	vec3<float> GetUp() const noexcept { return up; }
	vec3<float> GetRight() const noexcept { return right; }

	void SetEyePosition(vec3<float> newPos) noexcept { eye = newPos; }
	void SetFront(const vec3<float>& value) noexcept;
	void SetUp(vec3<float>& value) { up = value; }

	//void ResetUp(vec2<float> camera_up = { 0, 1 }) noexcept;

	void MoveX(float distance) noexcept;
	void MoveY(float distance) noexcept;
	void MoveZ(float distance) noexcept;

	//mat4<float> CameraToWorld() const noexcept;
	//mat4<float> WorldToCamera() const noexcept;

	//void Rotate(float angle_radians) noexcept;
	//vec3<float> rotate_by(float angle_in_radians, vec2<float> v) noexcept
	//{
	//	float cos_value = cos(angle_in_radians);
	//	float sin_value = sin(angle_in_radians);
	//	return vec3<float>{ cos_value * v.x - sin_value * v.y, sin_value * v.x + cos_value * v.y };
	//}

	// LookAt, eye space, camera space, view space
	mat4<float> BuildViewMatrix() const noexcept;

	float yaw = -ANGLE::pi / 2.f;
	float pitch = 0.f;
	float roll = 0.f;

	void UniformRegistry(GlobalUniformData& data);

private:
	vec3<float> eye{ 0.0f, 0.0f, 1.0f }; // camera position
	//vec3<float> target{ 0.0f, 0.0f, 0.0f };		// camera look at

	vec3<float> front = { 0.0f, 0.0f, -1.0f };
	vec3<float> up{ 0.0f, 1.0f, 0.0f };
	vec3<float> right{ 1.0f, 0.0f, 0.0f };
};
