#pragma once

#include "GUIImage.h"

enum CheckType {
	TICK, CROSS
};

/*
 A simple checkbox. Extends GUIImage, not GUIElement.
 */
class Checkbox : public GUIImage {
	public:
		Checkbox(const std::string& id, Vector2& position);
		Checkbox(const std::string& id, Vector2& position, bool isTicked, CheckType checkType);

		/*
		 Ticks this checkbox.
		 */
		void setTicked(bool ticked);

		/*
		 Returns true if this checkbox is ticked.
		 */
		bool isTicked();

		// void setBoolean(bool* tickBoolean); // Nahh, overdoing it a bit

		void mousePressed(MouseButton mousebutton, Vector2& mousePosition);
		void mouseReleased(MouseButton mouseButton, Vector2& mousePosition);

	private:
		GUIImage* p_tickImage;
		bool ticked;
};