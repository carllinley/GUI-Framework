#pragma once

#include "Button.h"
#include "ComboBox.h"
#include "Slider.h"
#include "Checkbox.h"

/*
 Includes several methods to respond to changes and events pertaining to GUI elements.
 Must be extended and used to define a 'state' for the GUI.
 */
class GUIState : public Lineage {
	friend class GUI;

	public:
		/*
		 stateID - this state's unique ID.
		 p_gui - a pointer to GUI for convenience.
		 */
		GUIState(const std::string& stateID, GUI* p_gui);

		/*
		 Responds to buttons being pressed.
		 */
		virtual void buttonPressed(Button& button);

		/*
		 Reponds to changes in the value of combo boxes.
		 */
		virtual void valueUpdate(ComboBox& comboBox, const std::string& selectedValue);

		/*
		 Responds to changes in the contents of text fields.
		 */
		virtual void valueUpdate(TextField& textField, const std::string& text);
		
		/*
		 Called when return is hit on a text field.
		 */
		virtual void onReturn(TextField& textField, const std::string& text);

		/*
		 Responds to changes in the value of sliders.
		 */
		virtual void valueUpdate(Slider& slider, int index);

		/*
		 Responds to changes in checkboxes.
		 */
		virtual void valueUpdate(Checkbox& checkbox, bool isChecked);

	protected:
		GUI* p_gui;
};