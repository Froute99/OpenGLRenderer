/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	
 *	File_name: Clock.hpp
 *	
 *	Clock class that helps calculate time
 *	
 *	2019/10/31
 */

#pragma once
#include <chrono>

class Clock
{
public:
	using time = std::chrono::high_resolution_clock;
	using second = std::chrono::duration<float>;

	Clock() noexcept
		: timePassed(0), timeStamp(time::now()) {}
	void UpdateClock() noexcept
	{
		timePassed += GetTimeFromLastUpdate();
		timeStamp = time::now();
	}
	
	float GetTimeFromLastUpdate() const noexcept
	{
		return std::chrono::duration<float>(time::now() - timeStamp).count();
	}
public:
	float timePassed;
private:
	time::time_point timeStamp;
	//float timePassed;
};
