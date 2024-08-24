/*
 *	Author: JeongHak Kim	froute99619@gmail.com
 *	File_name: vec4.hpp
 *
 *	Custom math library for vector has 4 element
 *
 *	Aug.23 2024
 */

#pragma once

template <typename T>
class vec4
{
public:
	constexpr vec4() noexcept
		: x(0), y(0), z(0), w(0) {}

	constexpr vec4(T x, T y, T z, T w) noexcept
		: x(x), y(y), z(z), w(w) {}

	constexpr vec4(T repeatedValue) noexcept
		: x(repeatedValue), y(repeatedValue), z(repeatedValue), w(repeatedValue) {}
	T x, y, z, w;
};

template <typename T>
constexpr void operator+=(vec4<T>& v, const vec4<T>& addingVector) noexcept
{
	v.x += addingVector.x;
	v.y += addingVector.y;
	v.z += addingVector.z;
	v.w += addingVector.w;
}

template <typename T>
constexpr void operator-=(vec4<T>& v, const vec4<T>& subtractingVector) noexcept
{
	v.x -= subtractingVector.x;
	v.y -= subtractingVector.y;
	v.z -= subtractingVector.z;
	v.w -= subtractingVector.w;
}

template <typename T>
constexpr void operator*=(vec4<T>& v, T scale) noexcept
{
	v.x *= scale;
	v.y *= scale;
	v.z *= scale;
	v.w *= scale;
}

template <typename T>
constexpr void operator/=(vec4<T>& v, const T divisor) noexcept
{
	assert(divisor != 0.f);
	v.x /= divisor;
	v.y /= divisor;
	v.z /= divisor;
	v.w /= divisor;
}

template <typename T>
constexpr vec4<T> operator-(const vec4<T>& v) noexcept
{
	return vec4<T>{ -v.x, -v.y, -v.z, -v.w };
}

template <typename T>
constexpr vec4<T> operator+(const vec4<T>& v1, const vec4<T>& v2) noexcept
{
	return vec4<T>{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
}

template <typename T>
constexpr vec4<T> operator-(const vec4<T>& v1, const vec4<T>& v2) noexcept
{
	return vec4<T>{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
}

template <typename T>
constexpr vec4<T> operator*(const vec4<T>& v, const T scale) noexcept
{
	return vec4<T>{ v.x * scale, v.y * scale, v.z * scale, v.w * scale };
}

template <typename T>
constexpr vec4<T> operator*(const T scale, const vec4<T>& v) noexcept
{
	return v * scale;
}

template <typename T>
constexpr vec4<T> operator/(const vec4<T>& v, T divisor) noexcept
{
	assert(divisor != 0.f);
	return vec4<T>{ v.x / divisor, v.y / divisor, v.z / divisor, v.w / divisor };
}

template <typename T>
constexpr bool operator==(const vec4<T>& v1, const vec4<T>& v2) noexcept
{
	if (abs(v1.x - v2.x) <= std::numeric_limits<T>::epsilon()
		&& abs(v1.y - v2.y) <= std::numeric_limits<T>::epsilon()
		&& abs(v1.z - v2.z) <= std::numeric_limits<T>::epsilon()
		&& abs(v1.w - v2.w) <= std::numeric_limits<T>::epsilon())
	{
		return true;
	}
	return false;
}

template <typename T>
constexpr bool operator!=(const vec4<T>& v1, const vec4<T>& v2) noexcept
{
	if (v1 == v2)
	{
		return false;
	}
	return true;
}

namespace Vector4
{
	template <typename T>
	constexpr T dot_product(const vec4<T>& v1, const vec4<T>& v2) noexcept
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	// no cross product exists in 4 dimensional space: https://math.stackexchange.com/questions/720813/do-four-dimensional-vectors-have-a-cross-product-property
	template <typename T>
	constexpr T cross_product(const vec4<T>& v1, const vec4<T>& v2) noexcept
	{
		return vec4<T>{ v1.y* v2.z - v2.y * v1.z,
			v1.z* v2.x - v2.z * v1.x,
			v1.x* v2.y - v2.x * v1.y,
			0.0f };
	}

	template <typename T>
	constexpr T magnitude_squared(const vec4<T>& v) noexcept
	{
		return dot_product(v, v);
	}

	template <typename T>
	constexpr T magnitude(const vec4<T>& v) noexcept
	{
		return sqrt(magnitude_squared(v));
	}

	template <typename T>
	constexpr T distance_between(const vec4<T>& v1, const vec4<T>& v2) noexcept
	{
		return sqrt(magnitude_squared(v1 - v2));
	}

	template <typename T>
	constexpr T angle_between(const vec4<T>& v1, const vec4<T>& v2) noexcept
	{
		assert(magnitude(v1) != 0.f && magnitude(v2) != 0.f);
		return acos(dot_product(v1, v2) / (magnitude(v1) * magnitude(v2)));
	}

	template <typename T>
	constexpr vec4<T> normalize(const vec4<T>& v) noexcept
	{
		T m = magnitude(v);
		assert(m != 0);
		return v / m;
	}
}
