/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	
 *	File_name: main.cpp
 *	
 *	Graphic demo main file
 *	
 *	Nov.25 2019
 *******************************************************/

#include "Application.h"
#include "../Clock.hpp"

int main()
{
	Application app;
	Clock clock;
	float dt = 0.0f;
	
	while (app.IsRunning())
	{
		clock.UpdateClock();
		app.Update(dt);
		dt = clock.GetTimeFromLastUpdate();
	}

	return 0;
}