#pragma once
#include <Graphics/Transform.h>
#include <Math/mat4.hpp>

class Object
{
public:
	virtual void Draw() = 0;

	inline Transform* GetTransform() noexcept { return &transform; }
	mat4<float>		  GetModelToWorld() { return transform.BuildModelMatrix(); }

	void Move(const vec3<float>& v) noexcept { transform.SetTranslation(transform.GetTranslation() + v); }
	void Rotate(const vec3<float>& v) noexcept { transform.SetRotation(transform.GetRotation() + v); }

private:
	Transform transform;

};
