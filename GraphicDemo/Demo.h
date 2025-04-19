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
#include <Graphics/OpenGLWindow.h>
#include <Graphics/Camera.h>
#include <Graphics/CameraView.h>
#include <Graphics/EventHandler.h>

class Demo : public SimpleEventHandler
{
public:
	explicit Demo(OpenGLWindow& window)
		: width(window.GetWindowWidth()), height(window.GetWindowHeight()) { Initialize(); }
	virtual void Initialize();
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