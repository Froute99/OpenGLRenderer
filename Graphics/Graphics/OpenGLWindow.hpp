/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: OpenGLWindow.hpp
 *	
 *	OpenGLWindow class which implemented with pimpl
 *	
 *	Fall 2019
 *	Sep.21 2019
 */

#pragma once
#include <memory>

class EventHandler;
class PlatformWindow;

class OpenGLWindow
{
public:
	OpenGLWindow() noexcept;
	~OpenGLWindow() noexcept;

	bool CanCreateWindow(int width, int height, EventHandler* event_handler, const char* title) noexcept;
	void SwapBuffers() noexcept;
	void PollEvents() noexcept;

	void ToggleVSync(bool status) noexcept;
	bool IsVSyncOn() noexcept;
	void ToggleFullScreen() noexcept;
	bool IsFullScreen() noexcept;

	void SetWindowTitle(const char* title) const noexcept;
	void SetWindowIcon() const noexcept;

	int GetWindowWidth() const noexcept;
	void SetWindowWidth(int new_width) noexcept;
	int GetWindowHeight() const noexcept;
	void SetWindowHeight(int new_height) noexcept;
private:
	std::unique_ptr<PlatformWindow> platformWindow;
};