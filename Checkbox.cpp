#include "Checkbox.h"
#include "GUI.h"

Checkbox::Checkbox(const std::string& id, Vector2& position) : GUIImage(id, "images/CHECKBOX.tga") {
	p_tickImage = new GUIImage(id + "_check", "images/CHECK_TICK.tga");
	p_tickImage->setResponsive(false);
	attachChild(p_tickImage);
	setLocalTranslation(position);

	setTicked(false);
}

Checkbox::Checkbox(const std::string& id, Vector2& position, bool isTicked, CheckType checkType) : GUIImage(id, "images/CHECKBOX.tga") {
	ticked = isTicked;

	p_tickImage = new GUIImage(id + "_check", (checkType == TICK ? "images/CHECK_TICK.tga" : "images/CHECK_CROSS.tga"));
	p_tickImage->setResponsive(false);
	attachChild(p_tickImage);
	setLocalTranslation(position);

	setTicked(isTicked);
}

void Checkbox::setTicked(bool ticked) {
	this->ticked = ticked;
	p_tickImage->setVisible(ticked);

	if (GUI::get().getState() != nullptr) {
		GUI::get().getState()->valueUpdate(*this, ticked);
	}
}

bool Checkbox::isTicked() {
	return ticked;
}

void Checkbox::mousePressed(MouseButton mousebutton, Vector2& mousePosition) {
}

void Checkbox::mouseReleased(MouseButton mouseButton, Vector2 & mousePosition) {
	setTicked(!ticked);
}