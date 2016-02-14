#pragma once

#include "Vector2.h"

enum KeyState {DOWN, UP};

/*
 Allows an element to respond to key presses.
 */
class KeyListener {
	public:
		virtual void keyPressed(int keyID) = 0;
		virtual void keyReleased(int keyID) = 0;
		virtual void charPressed(char keyCharacter) = 0;
		virtual void charReleased(char keyCharacter) = 0;
};