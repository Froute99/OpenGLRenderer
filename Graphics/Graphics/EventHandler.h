/*
 *	Author: JeongHak Kim	junghak.kim@digipen.edu
 *	File_name: EventHandler.hpp
 *	
 *	Event handler
 *	
 *	Fall 2019
 *	Oct.03 2019
 */

#pragma once

enum class KeyboardButton
{
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O,
	P, Q, R, S, T, U, V, W, X, Y, Z,
	Arrow_Left,
	Arrow_Right,
	Arrow_Up,
	Arrow_Down,
	Page_Down,
	Page_Up,
	Enter,
	Escape,
	Backspace
};

enum class MouseButton
{
	LEFT_PRESS,
	LEFT_RELEASE
};

class EventHandler
{
public:
	virtual void HandleKeyPress(KeyboardButton button) = 0;
	virtual void HandleKeyRelease(KeyboardButton button) = 0;
	virtual void HandleResizeEvent(const int& new_width, const int& new_height) = 0;
	virtual void HandleScrollEvent(float scroll_amount) = 0;
	virtual void HandleMousePositionEvent(float xpos, float ypos) = 0;
	virtual void HandleMouseEvent(MouseButton button) = 0;
	virtual void HandleWindowClose() = 0;
	virtual void HandleFocusEvent(bool focused) = 0;
};

class SimpleEventHandler : public EventHandler
{
public:
	void HandleKeyPress(KeyboardButton /*button*/) override {}
	void HandleKeyRelease(KeyboardButton /*button*/) override {}
	void HandleResizeEvent(const int& /*width*/, const int& /*height*/) override {}
	void HandleScrollEvent(float /*scroll_amount*/) override {};
	void HandleMousePositionEvent(float /*xpos*/, float /*ypos*/) override {}
	void HandleMouseEvent(MouseButton /*button*/) override {}
	void HandleWindowClose() override {}
	void HandleFocusEvent(bool /*focused*/) override {}
};