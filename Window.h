#pragma once

#include "GUIElement.h"
#include "Vector2.h"
#include "GUIImage.h"
#include "Button.h"

#include <string>

class Window : public GUIElement {
	public:
		Window(const std::string& id, Vector2& position, Vector2& dimensions);
		Window(const std::string& id, Vector2& position, Vector2& dimensions, ColourRGBA& backgroundColour);
		Window(const std::string& id, Vector2& position, Vector2& dimensions, const std::string& backgroundImage);

		void createCloseButton();

		void render();

		void resize(float width, float height);

	private:
		GUIImage* p_backgroundImage;
		Button* p_closeButton;
};