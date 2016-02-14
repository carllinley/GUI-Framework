#pragma once

#include "GUIElement.h"

class GUIImage;

/*
 Creates a scrollable area.
 */
class ScrollArea : public GUIElement {
	public:

		/*
		 Creates a new empty scrollable area using the specified position and dimensions.
		 */
		ScrollArea(const std::string id, Vector2& localTranslation, Vector2& dimensions);
		~ScrollArea();

		void render();
		void setBackground(const std::string& filePath);

	private:
		Vector2 dimensions = Vector2();

		GUIImage* p_backgroundImage;
};