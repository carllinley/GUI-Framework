#pragma once

#include "Vector2.h"
#include "Button.h"
#include "GUIImage.h"

enum Orientation {HORIZONTAL, VERTICAL};

/*
 A simple slider - can be horizontal or vertical.
 Uses 100 steps by default.
 */
class Slider : public GUIElement {

	public:
		/*
		 Creates a new slider using the specified settings.
		 width - the width of the slider. 230 gives the best results.
		 orientation - the orientation of the slider, HORIZTONAL or VERITCAL.
		 */
		Slider(const std::string& id, Vector2& localTranslation, float width, Orientation orientation);

		/*
		 Overridden to offset the rotation if necessary
		 */
		GUIElement* attachChild(GUIElement* child);

		/*
		 Sets the current index.
		 */
		void setIndex(int index);

		/*
		 Sets the number of 'steps' - indexes - that the slider uses.
		 Default is 100.
		 */
		void setStepCount(int numberOfSteps);

		/*
		 Returns the index currently selected.
		 */
		int getSelectedIndex();

		void onHoverOff(Vector2& mousePosition);
		void mousePressed(MouseButton mousebutton, Vector2& mousePosition);
		void mouseReleased(MouseButton mouseButton, Vector2& mousePosition);
		void mouseMoved(Vector2& mouseMoved);

	private:
		GUIImage* p_marker;
		Orientation orientation;
		float width;
		bool isSliding;

		int selectedIndex = 100, stepCount = 100;

		/*
		 Calculates the selected index and repositions the slider accordingly.
		 */
		void update(Vector2& mousePosition);
};