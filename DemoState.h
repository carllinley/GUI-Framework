#pragma once

#include "GUIState.h"
#include "Animation.h"
#include "Slider.h"
#include "GUIImage.h"
#include "Window.h"
#include "Checkbox.h"
#include "Chatbox.h"

class DemoState : public GUIState {
	public:
		DemoState(const std::string& stateID, GUI* gui);

		void test(); // Testing

		Button* createNiceButton(const std::string& id, Vector2& position, const std::string& label);
		
		void createAnimationWindow();

		void createImage(const std::string& imagePath);

		void createRGBAWindow();

		void createEffectsWindow();
		ComboBox* createEffectsComboBox(const std::string& id, Vector2& position);
		Effect* getEffectForName(const std::string& name);

		void createPlainWindow();


		void buttonPressed(Button& button);
		void valueUpdate(ComboBox& comboBox, const std::string& selectedValue);
		void valueUpdate(TextField& textField, const std::string& input);
		void onReturn(TextField& textField, const std::string& text);
		void valueUpdate(Slider& slider, int index);
		void valueUpdate(Checkbox& checkbox, bool isChecked);

	private:
		Animation* doomGuy;
		int doomGuyAnimation = 0;

		float effectSpeed;

		// Status
		Label* p_statusLabel;

		// RGBA window
		Slider* rSlider;
		Slider* gSlider;
		Slider* bSlider;
		Slider* aSlider;

		Window* windowRGBA;

		// Effects window
		Window* p_effectsWindow;
		ComboBox* generalEffect;
		ComboBox* onHoverEffect;
		ComboBox* onHoverOffEffect;

		Window* plainWindow;
		GUIImage* image;

		Chatbox* demoChatbox;
};