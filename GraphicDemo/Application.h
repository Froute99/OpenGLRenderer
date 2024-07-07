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
#include <array>
#include "OpenGLWindow.hpp"
#include "Camera.hpp"
#include "CameraView.hpp"
#include "EventHandler.hpp"
#include "Demo.hpp"

class Application : public SimpleEventHandler
{
public:
	enum DEMOINDEX
	{
		COMPUTESHADERDEMO,
		SHAPEDRAWING,
		TEXTUREDRAWING,
		ANIMATION,
		TRANSFORMPARENTDEMO,
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
	Camera camera;
	CameraView view;
	const int width = 1280;
	const int height = 720;

	int demoIndex = SHAPEDRAWING;
	std::array<std::unique_ptr<Demo>, DEMOSIZE> demo;
};