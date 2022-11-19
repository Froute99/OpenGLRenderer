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
#include "vec2.hpp"
#include "mat3.hpp"

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
	mat3<float>         GetCameraToNDCTransform() const noexcept { return cameraToNDC; }
	void                       SetFrameOfReference(FrameOfReference frame_of_reference) noexcept;
	constexpr FrameOfReference GetFrameOfReference() const noexcept { return frameOfReference; }

	void            SetViewSize(vec2<float> windowSize) noexcept;
	void            SetViewSize(int pixel_width, int pixel_height) noexcept;
	void            SetZoom(float new_zoom) noexcept;
	constexpr float GetZoom() const noexcept { return zoom; }

private:
	vec2<float> displaySize;
	mat3<float> cameraToNDC = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	float zoom{1.0f};
	FrameOfReference frameOfReference = FrameOfReference::RightHanded_OriginCenter;
};