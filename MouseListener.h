#pragma once

#include "Vector2.h"

enum MouseButton {NA, LEFT, RIGHT, MIDDLE};
enum MouseAction {PRESSED, RELEASED, MOVED};
enum MouseWheel {WHEEL_UP, WHEEL_DOWN};

/*
 Enables an element to respond to mouse actions.
 */
class MouseListener {
	public:
		virtual void mousePressed(MouseButton mouseButton, Vector2& mousePosition) = 0;
		virtual void mouseReleased(MouseButton mouseButton, Vector2& mousePosition) = 0;

		virtual void mouseMoved(Vector2& mousePosition) = 0;

		// void setAction(MouseButton mouseButton, MouseAction mouseAction, void* method);
};