#pragma once

#include "Button.h"
#include "GUIElement.h"
#include "ScrollArea.h"

#include <string>

class ScrollBar : public GUIElement {
	friend class ScrollPane;

	public:
		ScrollBar(const std::string& id, Vector2& localTranslation, float size);

		void scrollDown(float amount);
		void scrollUp(float amount);

		void mouseWheelUp(Vector2& mousePosition);
		void mouseWheelDown(Vector2& mousePosition);

		void recalculateThumb(float high, float low, float pos, float viewportHeight);

		void update(float timePerFrame);

	private:
		Button* p_up;
		Button* p_down;
		Button* p_track;
		Button* p_thumb;

		long lastUpdate;
		float size, jump;
};

/*
 Essentially a ScrollArea with a scrollbar.
 */
class ScrollPane : public GUIElement {
	friend class ScrollBar;

	public:
		ScrollPane(const std::string& id, Vector2& localTranslation, Vector2& dimensions);

		bool up(float amount);
		bool down(float amount);

		void floor();

		void setBackground(const std::string& filePath);

		void mouseWheelUp(Vector2& mousePosition);
		void mouseWheelDown(Vector2& mousePosition);

		// Overridden from Lineage - we need to keep p_track of the scroll area's internal dimensions
		GUIElement* attachChild(GUIElement* element);

		/*
		 Attaches a child to the pane itself, not its scrollable area.
		 */
		GUIElement* attachImmediateChild(GUIElement* element);

		ScrollBar* getScrollBar();

	private:
		float internalLow, internalHigh, scrollPos;
		GUIElement* p_scrollElementContainer;
		ScrollArea* p_scrollArea;
		ScrollBar* p_scrollBar;

		void updateThumb();
};