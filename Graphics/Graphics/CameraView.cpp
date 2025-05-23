/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: CameraView.cpp
 *
 *	Camera view class
 *
 *	Fall 2019
 *	Oct.10 2019
 */

#include "CameraView.h"
#include <Math/Angle.hpp>

CameraView::CameraView(float newFOV, float newNear)
	: fov(newFOV), near(newNear)
{
}

void CameraView::SetViewSize(int width, int height) noexcept
{
	aspectRatio = (float)height / (float)width;
	projectionMatrix = BuildProjectionMatrix();
}

void CameraView::SetZoom(float newZoom) noexcept
{
	zoom = newZoom;
	projectionMatrix = BuildProjectionMatrix();
}

mat4<float> CameraView::BuildProjectionMatrix() const noexcept
{
	return Matrix4::GeneralProjectionMatrix(ANGLE::DegreeToRadian(fov), aspectRatio, near, far);

	//return Matrix4::InfiniteProjectionMatrix(ANGLE::DegreeToRadian(fov), aspectRatio, near);
	
	//mat4<float> projection(
	//	1.0f / (aspectRatio * tangent), 0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f / tangent, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 1.0f,
	//	0.0f, 0.0f, -2.f * near, 0.0f);

}
