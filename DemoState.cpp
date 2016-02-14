#include "DemoState.h"
#include "Window.h"
#include "GUI.h"
#include "Chatbox.h"
#include "ScrollPane.h"
#include "Effect.h"

#include <iostream>

DemoState::DemoState(const std::string& stateID, GUI* gui) : GUIState(stateID, gui) {
	gui->formatTooltip("fonts/copperplate_gothic.glf", 10, ColourRGBA::YELLOW);

	/* 
	 Create the options area
	 */
	GUIElement* optionsContainer = new GUIElement("optionsContainer", Vector2(-470, 200)); // Empty element, used as a container

	optionsContainer->attachChild(new Label("state_title", "Demonstration", 14, Vector2(0, 70)));
	optionsContainer->attachChild(new Label("state_subtitle", "Press some buttons!", 9, Vector2(0, 55)));

	ComboBox* cursorCombo = new ComboBox("cursorCombo", Vector2(105, 40), 200);
	cursorCombo->attachChild(new Label("cursorLabel", "Cursor:", Vector2(-55, 11)));
	cursorCombo->setOptions(6, "Standard", "Arrow", "Diamond sword", "Dragon scimitar", "Portal gun", "Sonic screwdriver");
	optionsContainer->attachChild(cursorCombo);

	cursorCombo->getDescendant("option_Standard")->setToolTip("Bug: might not re-appear.");
	cursorCombo->getDescendant("option_Portal gun")->setToolTip("Animated!");

	ComboBox* imageCombo = new ComboBox("imageCombo", Vector2(85, 0), 150);
	imageCombo->attachChild(new Label("imageLabel", "Image:", Vector2(-45, 11)));
	imageCombo->setOptions(5, "Bat-smiley", "Derby", "Rage", "Red John", "You tried");
	imageCombo->setSelected("Red John");
	optionsContainer->attachChild(imageCombo);
	createImage("demo/images/redjohn.tga");

	Button* demoAnim = createNiceButton("demoAnim", Vector2(0, -58), "Animation");
	demoAnim->setToolTip("Opens the animation window.");
	demoAnim->resize(128, 37);
	optionsContainer->attachChild(demoAnim);

	optionsContainer->attachChild(createNiceButton("demoEffects", Vector2(0, -102), "Effects"))
	->setToolTip("Opens the effects window.");
	
	optionsContainer->attachChild(createNiceButton("demoRGBA", Vector2(0, -146), "Colour"))
	->setToolTip("Opens an RGBA colour picker.");

	optionsContainer->attachChild(createNiceButton("demoChatbox", Vector2(0, -190), "Chatbox"))
	->setToolTip("Demonstrates TextField and ScrollPane.");

	attachChild(optionsContainer);
}

void DemoState::createPlainWindow() {
	detachChild("plainWindow");

	plainWindow = new Window("plainWindow", Vector2(100, 100), Vector2(300, 200), ColourRGBA(ColourRGBA::CYAN));
	plainWindow->setDraggable(true);
	plainWindow->setResizable(true);
	plainWindow->setGradiant(TO_BOTTOM, 4.0f);

	ComboBox* gradiants = new ComboBox("gradiantCombo", Vector2(120, 80), 120);
	gradiants->setOptions(5, "To bottom", "To top", "To left", "To right", "None");
	gradiants->attachChild(new Label("gradLabel", "Gradiant:", 10, Vector2(-55, 10)));
	plainWindow->attachChild(gradiants);
	attachChild(plainWindow);
}

void DemoState::createRGBAWindow() {
	detachChild("rgbaWindow");

	windowRGBA = new Window("rgbaWindow", Vector2(), Vector2(270, 200), "images/WINDOW.tga");
	windowRGBA->attachChild(new Label("RGBALabel", "RGBA Colour", 10, ColourRGBA(ColourRGBA::WHITE), Vector2(10, 90)));

	rSlider = new Slider("rSlider", Vector2(20, 125), 230, HORIZONTAL);
	rSlider->setColour(ColourRGBA::RED);

	gSlider = new Slider("gSlider", Vector2(20, 90), 230, HORIZONTAL);
	gSlider->setColour(ColourRGBA::GREEN);

	bSlider = new Slider("bSlider", Vector2(20, 55), 230, HORIZONTAL);
	bSlider->setColour(ColourRGBA::BLUE);

	aSlider = new Slider("aSlider", Vector2(20, 20), 230, HORIZONTAL);
	aSlider->setColour(ColourRGBA(1, 1, 1, 0.5f));

	windowRGBA->attachChild(rSlider);
	windowRGBA->attachChild(gSlider);
	windowRGBA->attachChild(bSlider);
	windowRGBA->attachChild(aSlider);

	attachChild(windowRGBA);
}

/*
 Effects window
 */

void DemoState::createEffectsWindow() {
	detachChild("effectsWindow"); // the pointer, p_effectsWindow, will be invalid if the window has been detached

	p_effectsWindow = new Window("effectsWindow", Vector2(), Vector2(432, 306), "images/WINDOW.tga");
	p_effectsWindow->setResizable(true);

	p_effectsWindow->attachChild(new Label("effectsWindow_TitleLabel", "Effects", 14, Vector2(15, 140)));
	p_effectsWindow->attachChild(new Label("effectsWindow_SubtitleLabel", "Some effects to play with.", 9, Vector2(15, 125)));

	Button* resetButton = new Button("resetEffects", Vector2(160, 25), Vector2(), "Reset");
	resetButton->assignImages("images/BUTTON.tga", "images/BUTTON_HOVER.tga", "images/BUTTON_PRESSED.tga");
	resetButton->getLabel()->translate(15, 0);

	p_effectsWindow->attachChild(resetButton);

	// Speed slider
	Slider* speedSlider = new Slider("effects_speedSlider", Vector2(140, 70), 230, HORIZONTAL);
	speedSlider->setIndex(50); // Defaults 1.0f
	speedSlider->attachChild(new Label("effects_speedSliderLabel", "Speed:", 10, ColourRGBA(ColourRGBA::WHITE), Vector2(-38, 12)));
	p_effectsWindow->attachChild(speedSlider);

	// "On hover off" effect
	onHoverOffEffect = createEffectsComboBox("setOnHoverOffEffect", Vector2(140, 115));
	onHoverOffEffect->attachChild(new Label("onHoverOffLabel", "On leave:", 10, ColourRGBA(ColourRGBA::WHITE), Vector2(-52, 10)));
	p_effectsWindow->attachChild(onHoverOffEffect);

	// "On hover" effect
	onHoverEffect = createEffectsComboBox("setOnHoverEffect", Vector2(140, 155));
	onHoverEffect->attachChild(new Label("onHoverLabel", "On hover:", 10, ColourRGBA(ColourRGBA::WHITE), Vector2(-55, 10)));
	p_effectsWindow->attachChild(onHoverEffect);

	// "General" effect
	generalEffect = createEffectsComboBox("setGeneralEffect", Vector2(140, 195));
	generalEffect->attachChild(new Label("generalLabel", "General:", 10, ColourRGBA(ColourRGBA::WHITE), Vector2(-50, 10)));
	p_effectsWindow->attachChild(generalEffect);
	
	effectSpeed = 2.0f;
	attachChild(p_effectsWindow);
}

ComboBox* DemoState::createEffectsComboBox(const std::string& id, Vector2& position) { // Convenience method - creates a combo box of effects
	ComboBox* comboBox = new ComboBox(id, position, 180);
	comboBox->setOptions(9, "None", "ChangeColour", "FadeIn", "FadeOut", "Flash", "GrowIn", "Pulse", "Rotate", "ShrinkOut");
	return comboBox;
}

Effect* DemoState::getEffectForName(const std::string& name) {
	if (name == "ChangeColour") {
		return new ChangeColour(ColourRGBA::YELLOW);
	}
	if (name == "FadeIn") {
		return new FadeIn(effectSpeed);
	}
	if (name == "FadeOut") {
		return new FadeOut(effectSpeed);
	}
	if (name == "Flash") {
		return new Flash(effectSpeed);
	}
	if (name == "GrowIn") {
		return new GrowIn(effectSpeed);
	}
	if (name == "ShrinkOut") {
		return new ShrinkOut(effectSpeed);
	}
	if (name == "Pulse") {
		return new Pulse(effectSpeed);
	}
	if (name == "Rotate") {
		return new Rotate(effectSpeed);
	}
	return nullptr;
}

void DemoState::createImage(const std::string& imagePath) {
	detachChild("demoImage");

	image = new GUIImage("demoImage", imagePath);
	image->setLocalTranslation(Vector2(-503, -363));
	image->setDraggable(true);

	if (hasChild("rgbaWindow")) {
		float r = (rSlider->getSelectedIndex() != 0 ? rSlider->getSelectedIndex() / 100.0f : 0),
			  g = (gSlider->getSelectedIndex() != 0 ? gSlider->getSelectedIndex() / 100.0f : 0),
			  b = (bSlider->getSelectedIndex() != 0 ? bSlider->getSelectedIndex() / 100.0f : 0),
			  a = (aSlider->getSelectedIndex() != 0 ? aSlider->getSelectedIndex() / 100.0f : 0);

		image->setColour(ColourRGBA(r, g, b, a));
	}
	attachChild(image);
}

/*
 Animation window
 */

void DemoState::createAnimationWindow() {
	detachChild("animationWindow");

	Window* animWindow = new Window("animationWindow", Vector2(-500, 2), Vector2(368, 358), "images/WINDOW.tga");
	animWindow->setResizable(true);

	// Attaching the subtitle as a child of title gives weird results... strange
	animWindow->attachChild(new Label("aw_Title", "Animation", 14, Vector2(15, 165)));
	animWindow->attachChild(new Label("aw_Subtitle", "Configure the DOOM guy!", 9, Vector2(15, 150)));

	GUIImage* transArea = new GUIImage("transArea", "images/SCROLL_AREA.tga", Vector2(148, 148));
	transArea->setLocalTranslation(Vector2(110, 100));
	animWindow->attachChild(transArea);

	// Create the Doom guy amimation
	doomGuy = new Animation("DOOMGUY!", "demo/doomguy.tga", 64, new int[6]{ 3, 3, 3, 3, 3, 6 });
	doomGuy->setLocalTranslation(Vector2(10, 10));
	doomGuy->resize(128.0f, 128.0f);
	doomGuy->setUnordered(true);
	transArea->attachChild(doomGuy);

	// Unordered checkbox
	Checkbox* checkUnordered = new Checkbox("animation_checkUnordered", Vector2(60, 40), true, TICK);
	checkUnordered->setToolTip("If ticked, the animation plays in a random order.");
	checkUnordered->attachChild(new Label("unordered_label", "Unordered", 10, Vector2(15, 9)));
	animWindow->attachChild(checkUnordered);

	// Looping checkbox
	Checkbox* checkLooping = new Checkbox("animation_checkLoops", Vector2(60, 70), true, CROSS);
	checkUnordered->setToolTip("If ticked, plays in a random order.");
	checkLooping->attachChild(new Label("loop_label", "Loops", 10, Vector2(15, 9)));
	animWindow->attachChild(checkLooping);

	// Animation slider
	Slider* animationSlider = new Slider("animSlider", Vector2(180, 130), 230, VERTICAL);
	animationSlider->setStepCount(5);
	animWindow->attachChild(animationSlider);

	// Speed slider
	Slider* animationSpeedSlider = new Slider("animSpeed", Vector2(215, 130), 230, VERTICAL);
	animationSpeedSlider->setIndex(50); // 500ms by default
	animWindow->attachChild(animationSpeedSlider);

	// Attach the window
	attachChild(animWindow);
}

Button* DemoState::createNiceButton(const std::string& id, Vector2& position, const std::string& label) {
	Button* button = new Button(id, position, Vector2(108, 32), std::string(label));
	button->assignImages("images/BUTTON.tga", "images/BUTTON_HOVER.tga", "images/BUTTON_PRESSED.tga");
	return button;
}

/*
 Responding to events
 */

void DemoState::buttonPressed(Button& button) {
	if (button == "resetEffects") {
		generalEffect->setSelected("None");
		onHoverEffect->setSelected("None");
		onHoverOffEffect->setSelected("None");
		image->setVisible(true);
	}
	if (button == "demoAnim") {
		createAnimationWindow();
	}
	if (button == "demoEffects") {
		createEffectsWindow();
	}
	if (button == "demoRGBA") {
		createRGBAWindow();
	}
	if (button == "demoChatbox") {
		detachChild("chatbox");
		demoChatbox = new Chatbox("chatbox", Vector2(-17, -363), 520);
		attachChild(demoChatbox);

		demoChatbox->pushMessage("HAL-9000", "Hello.");
	}
	if (button == "demoPlain") {
		createPlainWindow();
	}
}

void DemoState::valueUpdate(ComboBox& comboBox, const std::string& selectedValue) {
	if (comboBox == "imageCombo") {
		if (selectedValue == "Bat-smiley") {
			createImage("demo/images/batsmiley.tga");
		}
		if (selectedValue == "Derby") {
			createImage("demo/images/derby.tga");
		}
		if (selectedValue == "Rage") {
			createImage("demo/images/rage.tga");
		}
		if (selectedValue == "Red John") {
			createImage("demo/images/redjohn.tga");
		}
		if (selectedValue == "You tried") {
			createImage("demo/images/youtried.tga");
		}
	}
	if (comboBox == "cursorCombo") {
		if (selectedValue == "Standard") {
			p_gui->setCursor("");
		}
		if (selectedValue == "Arrow") {
			p_gui->setCursor("images/cursor/arrow.tga");
		}
		if (selectedValue == "Diamond sword") {
			p_gui->setCursor("images/cursor/minesword.tga");
		}
		if (selectedValue == "Dragon scimitar") {
			p_gui->setCursor("images/cursor/runescim.tga");
		}
		if (selectedValue == "Sonic screwdriver") {
			p_gui->setCursor("images/cursor/dwsonic.tga");
		}
		if (selectedValue == "Portal gun") {
			p_gui->setAnimatedCursor("images/cursor/portalgun.tga", 2, 500, false);
		}
	}
	if (comboBox == "setGeneralEffect") {
		image->addEffect(GENERIC, getEffectForName(selectedValue));
	}
	if (comboBox == "setOnHoverEffect") {
		image->addEffect(TRIG_HOVER, getEffectForName(selectedValue));
	}
	if (comboBox == "setOnHoverOffEffect") {
		image->addEffect(TRIG_HOVER_OFF, getEffectForName(selectedValue));
	}
	if (comboBox == "gradiantCombo") {
		if (selectedValue == "To bottom") {
			plainWindow->setGradiant(TO_BOTTOM, 4.0f);
		}
		if (selectedValue == "To top") {
			plainWindow->setGradiant(TO_TOP, 4.0f);
		}
		if (selectedValue == "To left") {
			plainWindow->setGradiant(TO_LEFT, 4.0f);
		}
		if (selectedValue == "To right") {
			plainWindow->setGradiant(TO_RIGHT, 4.0f);
		}
		if (selectedValue == "None") {
			plainWindow->setGradiant(NO_GRADIANT);
		}
	}
}

void DemoState::valueUpdate(TextField& textField, const std::string& text) {
	// std::cout << "Changed " << textField->getID() << ", text: '" << textField->getText() << "'" << std::endl;
}

void DemoState::onReturn(TextField& textField, const std::string& text) {
	demoChatbox->pushMessage("Player", text);
	textField.clear();
}

void DemoState::valueUpdate(Slider& slider, int index) {
	if (slider == "rSlider" || slider == "gSlider"
	 || slider == "bSlider" || slider == "aSlider") {
		float r = (rSlider->getSelectedIndex() != 0 ? rSlider->getSelectedIndex() / 100.0f : 0),
			  g = (gSlider->getSelectedIndex() != 0 ? gSlider->getSelectedIndex() / 100.0f : 0),
			  b = (bSlider->getSelectedIndex() != 0 ? bSlider->getSelectedIndex() / 100.0f : 0),
			  a = (aSlider->getSelectedIndex() != 0 ? aSlider->getSelectedIndex() / 100.0f : 0);

		image->setColour(ColourRGBA(r, g, b, a));
	}
	else if (slider == "effects_speedSlider") {
		effectSpeed = index != 0 ? (float) index / 25 : 0;

		for (int i = GENERIC; i != TRIG_PRESSED; i++) {
			EffectTrigger trigger = static_cast<EffectTrigger>(i);

			if (image->getEffect(trigger) != nullptr) {
				image->getEffect(trigger)->setSpeed(effectSpeed);
			}
		}
	}
	else if (slider == "animSlider") {
		doomGuy->setAnimation(5 - index);

		if (index == 5) {
			slider.setColour(ColourRGBA::WHITE);
		}
		else {
			slider.setColour(ColourRGBA(0.2f * index, 0.0f, 0.0f, 1.0f));
		}
	}
	else if (slider == "animSpeed") {
		doomGuy->setDelay(index * 10);
	}
}

void DemoState::valueUpdate(Checkbox& checkbox, bool isChecked) {
	if (checkbox == "animation_checkUnordered") {
		doomGuy->setUnordered(isChecked);
	}
	if (checkbox == "animation_checkLoops") {
		doomGuy->setLooping(isChecked);
	}
}