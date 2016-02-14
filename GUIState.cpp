#include "GUIState.h"
#include "Slider.h"
#include "GUI.h"

#include <iostream>

GUIState::GUIState(const std::string& stateID, GUI* p_gui) {
	this->p_gui = p_gui;
	isState = true;
}

void GUIState::buttonPressed(Button& button) {
}

void GUIState::valueUpdate(ComboBox& comboBox, const std::string& selectedValue) {
}

void GUIState::valueUpdate(TextField& textField, const std::string& text) {
}

void GUIState::onReturn(TextField& textField, const std::string& text) {
}

void GUIState::valueUpdate(Slider& slider, int index) {
}

void GUIState::valueUpdate(Checkbox& checkbox, bool isChecked) {
}