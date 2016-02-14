#pragma once

#include "GUIElement.h"
#include "MouseListener.h"
#include "GUIImage.h"
#include "Label.h"

#include <vector>

enum ButtonEvent {IDLE, HOVER, PRESS};

/*
 A simple button.
 */
class Button : public GUIElement {
	friend class GUIElement;

	public:
		Button(const std::string& id, Vector2& localTranslation);
		Button(const std::string& id, Vector2& localTranslation, const std::string& imagePath);
		Button(const std::string& id, Vector2& localTranslation, Vector2& dimensions);
		Button(const std::string& id, Vector2& localTranslation, Vector2& dimensions, const std::string& labelText);
		Button(const std::string& id, Vector2& localTranslation, Vector2& dimensions, Label& label);
		Button(const std::string& id, Vector2& localTranslation, Vector2& dimensions, GUIImage& image);
		~Button();

		void render();
		void renderPostDescendants();

		void resize(float x, float y);
		
		void assignImages(const std::string& idleImage, const std::string& hoverImage, const std::string& pressedImage);
		void setOnPress(void(*onPress) (GUIElement& parentElement, Button& thisButton));

		/*
		 Sets the colour of the button's label to the specified colour whilst it is hovered over.
		 */
		void setHoverForegroundColour(ColourRGBA& colour);

		void setImage(ButtonEvent type, GUIImage* image);
		void setLabel(Label* label);

		Label* getLabel();

	protected:
		// Mouse actions
		void mousePressed(MouseButton mouseButton, Vector2& mousePosition);
		void mouseReleased(MouseButton mouseButton, Vector2& mousePosition);
		void onHover(Vector2& mousePosition);
		void onHoverOff(Vector2& mousePosition);

	private:
		std::unordered_map<int, GUIImage*> images;
		ButtonEvent currentState;

		void (*p_func_overrideOnPress) (GUIElement& parentElement, Button& thisButton);

		Label* p_label;
		ColourRGBA* p_standardColour;
		ColourRGBA* p_hoverColour;

		/*
		 Sets the visible image based on the current state of the button.
		 */
		void updateImage(ButtonEvent state);
};