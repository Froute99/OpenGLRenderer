/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: CameraView.hpp
 *
 *	Camera view class
 *
 *	Fall 2019
 *	Oct.10 2019
 */

#pragma once
#include <Math/mat4.hpp>

enum FrameOfReference
{
	RightHanded_OriginCenter,
	RightHanded_OriginBottomLeft,
	LeftHanded_OriginTopLeft,
	Normalize
};

class [[nodiscard]] CameraView
{
public:
	CameraView() = default;
	CameraView(float newFOV, float newNear);

	void            SetViewSize(int width, int height) noexcept;
	void			SetZoom(float newZoom) noexcept;
	constexpr float GetZoom() const noexcept { return zoom; }
	float			GetAspectRatio() const noexcept { return aspectRatio; }

	// Perspective
	mat4<float> BuildProjectionMatrix() const noexcept;

private:
	mat4<float> projectionMatrix;

	float aspectRatio = 1.0f;
	float fov = 90.0f;
	float near = 0.1f;

	float zoom = 1.0f;
};
