#pragma once

#include "TextField.h"
#include "Button.h"

struct ComboElement {
	const char* option;
	const Button* button;

	ComboElement(const char* option, const Button* button) {
		this->option = option;
		this->button = button;
	}
};

/*
 A simple combobox of strings.
 */
class ComboBox : public GUIElement {
	public:
		ComboBox(const std::string& id, Vector2& localTranslation, float width);
		~ComboBox();

		/*
		 Opens/closes this combo box's list of options.
		 */
		void toggleDropdown();

		void renderPostDescendants();

		/*
		 Sets the background of the drop-p_down menu.
		 */
		void setBackground(GUIImage* image);

		/*
		 Sets the font. Applies to the text and the options.
		 */
		void setFont(const std::string& filePath);

		/*
		 Sets the specified option as the selected option.
		 */
		void setSelected(const std::string& option);

		/*
		 Sets the options of the combo box.
		 All options must be a string, and the number of options must be specified in 'count'.
		 Not adhering to these points will give a weird-result and/or a lovely crash.
		 */
		void setOptions(int count, const char* ...);	// Seems ellipsis are bad practice

		void onFocus(Vector2& mousePosition);
		void onLostFocus(Vector2& mousePosition);

		/*
		 Adds the specified option to the list.
		 */
		void addOption(const char* option);

		/*
		 Removes the specified option from the list.
		 */
		void removeOption(const char* option);

		/*
		 Returns the current selected index.
		 */
		int getSelectedIndex();

		/*
		 Returns the current selected value.
		 */
		const char* getSelectedValue();

		bool isDropped();

	private:
		Button* fieldButton;
		Button* dropButton;
		GUIElement* p_buttonContainer; // Contains the buttons in the drop p_down menu
		std::unordered_map<int, ComboElement*> elements;
		std::string selected;

		bool droppedDown = false;

		float width;

		int optionCount = 0;
		int selectedIndex = 0;			// The currently selected index.

		Button* createButton(const char* option, int optionCount);
};