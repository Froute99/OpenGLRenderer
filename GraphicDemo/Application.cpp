/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	
 *	File_name: Application.cpp
 *	
 *	Graphic demo main file
 *	
 *	Nov.29 2019
 *******************************************************/

#include <iostream>
#include "glfw3.h"
#include "Application.h"
//#include "PhongShadingDemo.h"
#include "TextureDrawingDemo.h"
#include "AnimationDemo.h"
#include "TransformHierarchyDemo.h"
#include "ComputeShaderDemo.h"
#include "PBRdemo.h"
//#include "TexturedPBRDemo.h"
#include "TestStage.h"

bool Application::IsRunning() const
{
	return isRunning;
}

Application::Application()
{
	Initialize();
}

Application::~Application()
{
	ShutDown();
}

void Application::Initialize()
{
	if (!window.CanCreateWindow(width, height, this, "Graphic Demo"))
	{
		return;
	}
	isRunning = true;

	window.ToggleVSync(true);
	window.ToggleMouse();

	demo[PBR] = std::make_unique<PBRDemo>(window);
	//demo[PHONGSHADING] = std::make_unique<PhongShadingDemo>(window);
	//demo[TEXTUREDPBR] = std::make_unique<TexturedPBRDemo>(window);
	//demo[ANIMATION] = std::make_unique<AnimationDemo>(window);
	//demo[HIERARCHY_DEMO] = std::make_unique<TransformHierarchyDemo>(window);
	//demo[TEST] = std::make_unique<TestStage>(window);
	//demo[COMPUTESHADERDEMO] = std::make_unique<ComputeShaderDemo>(window);
}

void Application::Update(float dt)
{
	demo[demoIndex]->UpdateWrapper(dt);

	window.SwapBuffers();
	window.PollEvents();
}

void Application::ShutDown()
{
	isRunning = false;
}

void Application::HandleKeyPress(KeyboardButton button)
{
	demo[demoIndex]->HandleKeyPress(button);
	switch (button)
	{
	case KeyboardButton::Escape:
		ShutDown();
		break;
	case KeyboardButton::F:
		window.ToggleFullScreen();
		break;
	case KeyboardButton::V:
		window.ToggleVSync(!window.IsVSyncOn());
		break;
	case KeyboardButton::T:
		window.ToggleMouse();
		break;
	case KeyboardButton::Page_Up:
		demo[demoIndex]->ResetCamera();
		++demoIndex;
		if (demoIndex >= static_cast<int>(demo.size()))
		{
			demoIndex = 0;
		}
		break;
	case KeyboardButton::Page_Down:
		demo[demoIndex]->ResetCamera();
		if (demoIndex == 0)
		{
			demoIndex = static_cast<int>(demo.size()) - 1;
			break;
		}
		--demoIndex;
		break;
	default:
		break;
	}
}

void Application::HandleKeyRelease(KeyboardButton button)
{
	demo[demoIndex]->HandleKeyRelease(button);
}

void Application::HandleScrollEvent(float scroll_amount)
{
	demo[demoIndex]->HandleScrollEvent(scroll_amount);
}
void Application::HandleMousePositionEvent(float xpos, float ypos)
{
	if (window.GetMouseLock() == true)
		return;
	demo[demoIndex]->HandleMousePositionEvent(xpos, ypos);
}

void Application::HandleMouseEvent(MouseButton button)
{
	demo[demoIndex]->HandleMouseEvent(button);
}

void Application::HandleResizeEvent(const int new_width, const int new_height)
{
	window.SetWindowWidth(new_width);
	window.SetWindowHeight(new_height);

	demo[demoIndex]->HandleResizeEvent(new_width, new_height);
}

void Application::HandleWindowClose()
{
	ShutDown();
}

void Application::HandleFocusEvent(bool focused)
{
	demo[demoIndex]->HandleFocusEvent(focused);
}