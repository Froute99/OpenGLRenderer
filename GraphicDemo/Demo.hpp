/********************************************************
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *
 *	File_name: Demo.h
 *
 *	Graphic demo main file
 *
 *	Nov.28 2019
 *******************************************************/

#pragma once
#include <Graphics/OpenGLWindow.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/CameraView.hpp>
#include <Graphics/EventHandler.hpp>

class Demo : public SimpleEventHandler
{
public:
	explicit Demo(OpenGLWindow& window)
		: width(window.GetWindowWidth()), height(window.GetWindowHeight()) {}
	virtual void Initialize() = 0;
	virtual void Update(float dt) = 0;

	virtual void ResetCamera() = 0;

	virtual ~Demo() {}

	void HandleResizeEvent(const int& width, const int& height) override;
	void HandleScrollEvent(float scroll_amount) override;
	void HandleFocusEvent(bool focused) override;

	int GetScreenWidth() { return width; }
	int GetScreenHeight() { return height; }

protected:
	Camera	   camera;
	CameraView view;
	int		   width;
	int		   height;
	bool	   isFocused = true;
};