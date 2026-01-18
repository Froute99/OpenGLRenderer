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
#include <Graphics/UBO.h>

CameraView::CameraView(float newFOV, float newNear)
	: fov(newFOV), near(newNear)
{
}

void CameraView::SetViewSize(int width, int height) noexcept
{
	aspectRatio = (float)width / (float)height;
	projectionMatrix = BuildProjectionMatrix();
	GlobalUniformManager::GetInstance().SetDirty();
}

void CameraView::SetZoom(float newZoom) noexcept
{
	zoom = newZoom;
	projectionMatrix = BuildProjectionMatrix();
	GlobalUniformManager::GetInstance().SetDirty();
}

mat4<float> CameraView::BuildProjectionMatrix() const noexcept
{
	return Matrix4::GeneralProjectionMatrix(ANGLE::DegreeToRadian(fov), aspectRatio, near, far);
	//return Matrix4::InfiniteProjectionMatrix(ANGLE::DegreeToRadian(fov), aspectRatio, near);
}

void CameraView::UniformRegistry(GUD& data)
{
	data.projectionMatrix = BuildProjectionMatrix();
}
