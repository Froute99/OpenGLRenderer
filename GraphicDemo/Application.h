/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *
 *	File_name: Application.h
 *
 *	Graphic demo main file
 *
 *	Nov.29 2019
 *******************************************************/

#pragma once
#include <Graphics/OpenGLWindow.h>
#include <Graphics/Camera.h>
#include <Graphics/CameraView.h>
#include <Graphics/EventHandler.h>
#include "Demo.h"
#include <array>

class Application : public SimpleEventHandler
{
public:
	enum DEMOINDEX
	{
		SHAPEDRAWING,
		PBR,
		TEXTUREDPBR,
		// TEXTUREDRAWING,
		// ANIMATION,
		// TRANSFORMPARENTDEMO,
		// COMPUTESHADERDEMO,
		DEMOSIZE
	};

	Application();
	void Initialize();
	void Update(float dt);
	void ShutDown();

	bool IsRunning() const;

	void HandleKeyPress(KeyboardButton button) override;
	void HandleKeyRelease(KeyboardButton button) override;
	void HandleScrollEvent(float scroll_amount) override;
	void HandleMousePositionEvent(float xpos, float ypos) override;
	void HandleMouseEvent(MouseButton button) override;
	void HandleResizeEvent(const int& width, const int& height) override;
	void HandleWindowClose() override;
	void HandleFocusEvent(bool focused) override;

private:
	bool isRunning = false;

protected:
	OpenGLWindow window;
	Camera		 camera;
	CameraView	 view;
	const int	 width = 800;
	const int	 height = 800;

	int											demoIndex = 0;
	std::array<std::unique_ptr<Demo>, DEMOSIZE> demo;
};