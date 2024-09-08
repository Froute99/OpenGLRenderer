/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: PlatformWindowGLFW.cpp
 *	
 *	PlatformWindowGLFW class
 *	
 *	Fall 2019
 *	Sep.21 2019
 */

#include <iostream>
#include "GL/glew.h"
#include "PlatformWindowGLFW.hpp"
#include "PATH.hpp"
#include "EventHandler.hpp"
#include <stb_image.h>

EventHandler* eventHandler;

void frame_buffer_size_callback(GLFWwindow* /*window*/, int width, int height)
{
	glViewport(0, 0, width, height);
}

void resize_callback(GLFWwindow* /*window*/, int width, int height)
{
	eventHandler->HandleResizeEvent(width, height);
}

void window_close_callback(GLFWwindow* /*window*/)
{
	eventHandler->HandleWindowClose();
}

void key_callback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_A:
			eventHandler->HandleKeyPress(KeyboardButton::A);
			break;
		case GLFW_KEY_B:
			eventHandler->HandleKeyPress(KeyboardButton::B);
			break;
		case GLFW_KEY_C:
			eventHandler->HandleKeyPress(KeyboardButton::C);
			break;
		case GLFW_KEY_D:
			eventHandler->HandleKeyPress(KeyboardButton::D);
			break;
		case GLFW_KEY_E:
			eventHandler->HandleKeyPress(KeyboardButton::E);
			break;
		case GLFW_KEY_F:
			eventHandler->HandleKeyPress(KeyboardButton::F);
			break;
		case GLFW_KEY_G:
			eventHandler->HandleKeyPress(KeyboardButton::G);
			break;
		case GLFW_KEY_H:
			eventHandler->HandleKeyPress(KeyboardButton::H);
			break;
		case GLFW_KEY_I:
			eventHandler->HandleKeyPress(KeyboardButton::I);
			break;
		case GLFW_KEY_J:
			eventHandler->HandleKeyPress(KeyboardButton::J);
			break;
		case GLFW_KEY_K:
			eventHandler->HandleKeyPress(KeyboardButton::K);
			break;
		case GLFW_KEY_L:
			eventHandler->HandleKeyPress(KeyboardButton::L);
			break;
		case GLFW_KEY_M:
			eventHandler->HandleKeyPress(KeyboardButton::M);
			break;
		case GLFW_KEY_N:
			eventHandler->HandleKeyPress(KeyboardButton::N);
			break;
		case GLFW_KEY_O:
			eventHandler->HandleKeyPress(KeyboardButton::O);
			break;
		case GLFW_KEY_P:
			eventHandler->HandleKeyPress(KeyboardButton::P);
			break;
		case GLFW_KEY_Q:
			eventHandler->HandleKeyPress(KeyboardButton::Q);
			break;
		case GLFW_KEY_R:
			eventHandler->HandleKeyPress(KeyboardButton::R);
			break;
		case GLFW_KEY_S:
			eventHandler->HandleKeyPress(KeyboardButton::S);
			break;
		case GLFW_KEY_T:
			eventHandler->HandleKeyPress(KeyboardButton::T);
			break;
		case GLFW_KEY_U:
			eventHandler->HandleKeyPress(KeyboardButton::U);
			break;
		case GLFW_KEY_V:
			eventHandler->HandleKeyPress(KeyboardButton::V);
			break;
		case GLFW_KEY_W:
			eventHandler->HandleKeyPress(KeyboardButton::W);
			break;
		case GLFW_KEY_X:
			eventHandler->HandleKeyPress(KeyboardButton::X);
			break;
		case GLFW_KEY_Y:
			eventHandler->HandleKeyPress(KeyboardButton::Y);
			break;
		case GLFW_KEY_Z:
			eventHandler->HandleKeyPress(KeyboardButton::Z);
			break;
		case GLFW_KEY_LEFT:
			eventHandler->HandleKeyPress(KeyboardButton::Arrow_Left);
			break;
		case GLFW_KEY_RIGHT:
			eventHandler->HandleKeyPress(KeyboardButton::Arrow_Right);
			break;
		case GLFW_KEY_UP:
			eventHandler->HandleKeyPress(KeyboardButton::Arrow_Up);
			break;
		case GLFW_KEY_DOWN:
			eventHandler->HandleKeyPress(KeyboardButton::Arrow_Down);
			break;
		case GLFW_KEY_PAGE_DOWN:
			eventHandler->HandleKeyPress(KeyboardButton::Page_Down);
			break;
		case GLFW_KEY_PAGE_UP:
			eventHandler->HandleKeyPress(KeyboardButton::Page_Up);
			break;
		case GLFW_KEY_ENTER:
			eventHandler->HandleKeyPress(KeyboardButton::Enter);
			break;
		case GLFW_KEY_ESCAPE:
			eventHandler->HandleKeyPress(KeyboardButton::Escape);
			break;
		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			eventHandler->HandleKeyRelease(KeyboardButton::W);
			break;
		case GLFW_KEY_A:
			eventHandler->HandleKeyRelease(KeyboardButton::A);
			break;
		case GLFW_KEY_S:
			eventHandler->HandleKeyRelease(KeyboardButton::S);
			break;
		case GLFW_KEY_D:
			eventHandler->HandleKeyRelease(KeyboardButton::D);
			break;
		case GLFW_KEY_Z:
			eventHandler->HandleKeyRelease(KeyboardButton::Z);
			break;
		case GLFW_KEY_X:
			eventHandler->HandleKeyRelease(KeyboardButton::X);
			break;
		case GLFW_KEY_LEFT:
			eventHandler->HandleKeyRelease(KeyboardButton::Arrow_Left);
			break;
		case GLFW_KEY_RIGHT:
			eventHandler->HandleKeyRelease(KeyboardButton::Arrow_Right);
			break;
		case GLFW_KEY_UP:
			eventHandler->HandleKeyRelease(KeyboardButton::Arrow_Up);
			break;
		case GLFW_KEY_DOWN:
			eventHandler->HandleKeyRelease(KeyboardButton::Arrow_Down);
			break;
		}
	}
}

void mouse_button_callback(GLFWwindow*, int button, int action, int /*mods*/)
{
	if (action == GLFW_PRESS)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			eventHandler->HandleMouseEvent(MouseButton::LEFT_PRESS);
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			eventHandler->HandleMouseEvent(MouseButton::LEFT_RELEASE);
			break;
		}
	}
}

void scroll_callback(GLFWwindow*, double, double y_offset)
{
	eventHandler->HandleScrollEvent(float(y_offset));
}

static void cursor_position_callback(GLFWwindow*, double xpos, double ypos)
{
	eventHandler->HandleMousePositionEvent(float(xpos), float(ypos));
}

void window_focus_callback(GLFWwindow*, int focus)
{
	if (focus != GLFW_TRUE)
	{
		eventHandler->HandleFocusEvent(false);
	}
	else
	{
		eventHandler->HandleFocusEvent(true);
	}
}

bool PlatformWindow::CanCreateWindow(int width, int height, EventHandler* event_handler, const char* title) noexcept
{
	eventHandler = event_handler;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	if (!glfwInit())
	{
		glfwTerminate();
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, GL_TRUE);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	GLFWmonitor* monitor = nullptr;
	GLFWwindow* shareWindow = nullptr;

	window = glfwCreateWindow(width, height, title, monitor, shareWindow);

	if (!window)
	{
		glfwTerminate();
		return false;
	}

	windowSize[0] = width;
	windowSize[1] = height;

	ToggleVSync(true);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
	glfwSetWindowSizeCallback(window, resize_callback);
	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetWindowFocusCallback(window, window_focus_callback);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << glewGetErrorString(err);
		return false;
	}

	SetWindowIcon();

	return true;
}

bool PlatformWindow::IsVSyncOn() noexcept
{
	return isVSyncOn;
}

void PlatformWindow::ToggleVSync(bool status) noexcept
{
	isVSyncOn = status;
	glfwSwapInterval(isVSyncOn);
}

void PlatformWindow::SwapBuffers() noexcept
{
	glfwSwapBuffers(window);
}

void PlatformWindow::PollEvents() noexcept
{
	glfwPollEvents();
}

bool PlatformWindow::IsFullScreen() noexcept
{
	return isFullScreen;
}

void PlatformWindow::ToggleFullScreen() noexcept
{
	isFullScreen = !isFullScreen;
	if (!IsFullScreen())
	{
		glfwGetWindowPos(window, &posBackup[0], &posBackup[1]);
		glfwGetWindowSize(window, &sizeBackup[0], &sizeBackup[1]);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, 0);
	}
	else
	{
		glfwSetWindowMonitor(window, nullptr, posBackup[0], posBackup[1], sizeBackup[0], sizeBackup[1], 0);
	}
}

void PlatformWindow::SetWindowTitle(const char* title) const noexcept
{
	glfwSetWindowTitle(window, title);
}

void PlatformWindow::SetWindowIcon() const noexcept
{
	GLFWimage icon;
	icon.pixels = stbi_load(PATH::icon_image.generic_string().c_str() , &icon.width, &icon.height, 0, STBI_rgb_alpha);
	glfwSetWindowIcon(window, 1, &icon);
	stbi_image_free(icon.pixels);
}

int PlatformWindow::GetWindowWidth() const noexcept
{
	return windowSize[0];
}

int PlatformWindow::GetWindowHeight() const noexcept
{
	return windowSize[1];
}

void PlatformWindow::SetWindowWidth(int new_width) noexcept
{
	windowSize[0] = new_width;
}

void PlatformWindow::SetWindowHeight(int new_height) noexcept
{
	windowSize[1] = new_height;
}
