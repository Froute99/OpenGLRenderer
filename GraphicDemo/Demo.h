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

class Demo : public EventHandler
{
public:
	explicit Demo(OpenGLWindow& window)
		: width(window.GetWindowWidth()), height(window.GetWindowHeight()) { Initialize(); }
	virtual void Initialize();
	virtual void Update(float /*dt*/) = 0;
	void UpdateWrapper(float dt) noexcept;
	virtual void DrawGUI() = 0;

	virtual void ResetCamera() = 0;

	virtual ~Demo() {}

	//void HandleResizeEvent(const int& width, const int& height) override;
	//void HandleScrollEvent(float scroll_amount) override;
	//void HandleFocusEvent(bool focused) override;

	void HandleKeyPress(KeyboardButton key) override;
	void HandleKeyRelease(KeyboardButton key) override;
	void HandleResizeEvent(const int w, const int h) override;
	void HandleScrollEvent(float value) override;
	void HandleMousePositionEvent(float x, float y) override;
	void HandleMouseEvent(MouseButton mouse) override;
	void HandleWindowClose() override;
	void HandleFocusEvent(bool focused) override;

	int GetScreenWidth() { return width; }
	int GetScreenHeight() { return height; }

protected:
	Camera	   camera;
	CameraView view;
	vec3<float> cameraMovement;
	float		mousePast[2];
	
	int	 width;
	int	 height;
	bool isFocused = true;
	bool isFirstFrame = true;
};