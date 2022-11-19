/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: Vec2UnitTest.cpp
 *	
 *	Vec2 Unit Test
 *	
 *	Fall 2019
 *	Dec.04 2019
 */

#define CATCH_CONFIG_MAIN
#include <limits>
#include <catch.hpp>
#include <vec2.hpp>
#include <Angle.hpp>

namespace
{
	constexpr auto e = std::numeric_limits<float>::epsilon();
}

TEST_CASE("Vec2 Constructors")
{
	SECTION("Default")
	{
		constexpr vec2<float> v;
		REQUIRE((v.x == 0.0f && v.y == 0.0f));
	}
	SECTION("Two Parameter")
	{
		constexpr vec2<float> v{ 1.0f, 2.0f };
		REQUIRE((v.x == 1.0f && v.y == 2.0f));
	}
	SECTION("Repeated Value")
	{
		constexpr vec2<float> v{ 1.0f };
		REQUIRE((v.x == 1.0f && v.y == 1.0f));
	}
}

TEST_CASE("Vec2 Arithmetic Operators")
{
	constexpr vec2<float> v1{ 1.3f, 3.2f };
	constexpr vec2<float> v2{ 2.2f, 6.5f };
	vec2<float> result;
	SECTION("+")
	{
		result = v1 + v2;
		constexpr vec2<float> answer{ 3.5f, 9.7f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e));
	}
	SECTION("-")
	{
		result = v1 - v2;
		constexpr vec2<float> answer{ -0.9f, -3.3f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e));
	}
	SECTION("*")
	{
		result = v1 * 5.0f;
		constexpr vec2<float> answer{ 6.5f, 16.0f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e));
	}
	SECTION("/")
	{
		result = v1 / 2.0f;
		constexpr vec2<float> answer{ 0.65f, 1.6f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e));
	}
}

TEST_CASE("Vec2 Arithmetic with Assign Operators")
{
	vec2<float> v{ 0.8f, 1.3f };
	vec2<float> result{ 0.0f };
	SECTION("+=")
	{
		result += v;
		constexpr vec2<float> answer{ 0.8f, 1.3f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e));
	}
	SECTION("-=")
	{
		result -= v;
		constexpr vec2<float> answer{ -0.8f, -1.3f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e));
	}
	SECTION("*=")
	{
		v *= 2.0f;
		constexpr vec2<float> answer{ 1.6f, 2.6f };
		REQUIRE((abs(v.x - answer.x) <= e && abs(v.y - answer.y) <= e));
	}
	SECTION("/=")
	{
		v /= 2.0f;
		constexpr vec2<float> answer{ 0.4f, 0.65f };
		REQUIRE((abs(v.x - answer.x) <= e && abs(v.y - answer.y) <= e));
	}
}

TEST_CASE("Vec2 Logical Operators")
{
	constexpr vec2<float> v{ 1.3f, 1.0f };
	SECTION("-v")
	{
		constexpr vec2<float> result = -v;
		constexpr vec2<float> answer{ -1.3f, -1.0f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e));
	}
	SECTION("==")
	{
		bool isSame = (v == vec2<float>{ 1.3f, 1.0f });
		REQUIRE(isSame == true);
	}
	SECTION("!=")
	{
		bool isNotSame = (v != vec2<float>{ 1.3f, 2.0f });
		REQUIRE(isNotSame == true);
	}
}

TEST_CASE("Vec2 Other Functions")
{
	constexpr vec2<float> v1{ 1.2f, 1.5f };
	constexpr vec2<float> v2{ 0.8f, 1.3f };
	SECTION("Dot Product")
	{
		constexpr float result = dot_product(v1, v2);
		REQUIRE(abs(result - 2.91f) <= e * 2);
	}
	SECTION("Magnitude Squared")
	{
		constexpr float result = magnitude_squared(v1);
		REQUIRE(abs(result - 3.69f) <= e);
	}
	SECTION("Manitude")
	{
		const float result = magnitude(v1);
		REQUIRE(abs(result - 1.920937271229855f) <= e);
	}
	SECTION("Distance Between")
	{
		const float result = distance_between(v1, v2);
		REQUIRE(abs(result - 0.4472135954999579f) <= e);
	}
	SECTION("Angle Between")
	{
		const float result = angle_between(vec2<float>{ 1.0f, 1.0f }, { 0.0f, 1.0f });
 		REQUIRE(abs(result - ANGLE::pi / 4) <= e);
	}
	SECTION("Rotate By")
	{
		const vec2<float> result = rotate_by(ANGLE::pi, v1);
		constexpr vec2<float> answer = { -1.2f, -1.5f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e));
	}
	SECTION("Normalize")
	{
		const vec2<float> result = normalize(v1);
		constexpr vec2<float> answer = { 0.6246950475544241f, 0.7808688094430302f };
		REQUIRE((abs(result.x - answer.x) <= e && abs(result.y - answer.y) <= e));
	}
}
