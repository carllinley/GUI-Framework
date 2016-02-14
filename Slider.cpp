#include "Slider.h"
#include "GUI.h"

#include <cmath> // For rounding

Slider::Slider(const std::string& id, Vector2& localTranslation, float width, Orientation orientation) : GUIElement(id, localTranslation, Vector2(width, 24)) {
	if (orientation == VERTICAL) {
		setLocalRotation(90);
	}

	this->width = width - 25;
	this->orientation = orientation;

	p_marker = new GUIImage((id + "_track").c_str(), "images/SLIDER_MARKER.tga");
	GUIImage* track = new GUIImage((id + "_slider").c_str(), "images/SLIDER_TRACK.tga");

	track->setResponsive(false);
	p_marker->setResponsive(false);

	if (orientation == HORIZONTAL) {
		p_marker->setLocalTranslation(Vector2((this->width / stepCount) * selectedIndex, 0));
	}
	else {
		p_marker->setLocalTranslation(Vector2((this->width / stepCount) * selectedIndex, 0));
	}
	Lineage::attachChild(track);
	Lineage::attachChild(p_marker);
}

GUIElement* Slider::attachChild(GUIElement* child) {
	if (orientation == VERTICAL) {
		child->setLocalRotation(child->getLocalRotation() - 90); // Offset
	}
	return Lineage::attachChild(child);
}

void Slider::setIndex(int index) {
	if (index != selectedIndex&& GUI::get().getState() != nullptr) { // Front-end may need an update
		GUI::get().getState()->valueUpdate(*this, index);
	}

	selectedIndex = index;
	p_marker->setLocalTranslation(Vector2((width / stepCount) * selectedIndex, 0));
}

void Slider::update(Vector2& mousePosition) {
	int currentIndex = selectedIndex;
	Vector2* worldTranslation = getWorldTranslation();

	worldTranslation->setX(-worldTranslation->getX());

	Vector2 localCoordinates = *worldTranslation + mousePosition - (GUI::get().getDimensions() / 2.0f);

	if (orientation == HORIZONTAL) {
		localCoordinates.setY(0);

		if (localCoordinates.getX() > width) {
			localCoordinates.setX(width);
			selectedIndex = stepCount;
		}
		if (localCoordinates.getX() < 0) {
			localCoordinates.setX(0);
			selectedIndex = 0;
		}
		else {
			selectedIndex = (int) round(((localCoordinates.getX() * 100.0f / width) * (float) stepCount) / 100.0f);
		}
	}
	else {
		localCoordinates.setX(0);
		localCoordinates.setY(-localCoordinates.getY() + 75);

		if (localCoordinates.getY() > width) {
			localCoordinates.setY(width);
			selectedIndex = stepCount;
		}
		if (localCoordinates.getY() < 0) {
			localCoordinates.setY(0);
			selectedIndex = 0;
		}
		else {
			selectedIndex = (int) round(((localCoordinates.getY() * 100.0f / (float) width) * stepCount) / 100.0f);
		}
	}

	if (currentIndex != selectedIndex) { // Changed
		p_marker->setLocalTranslation(Vector2((width / stepCount) * selectedIndex, 0));

		if (GUI::get().getState() != nullptr) { // Front-end may need an update
			GUI::get().getState()->valueUpdate(*this, selectedIndex);
		}
	}
	delete worldTranslation;
}

void Slider::setStepCount(int numberOfSteps) {
	stepCount = numberOfSteps;

	if (selectedIndex > stepCount) {
		selectedIndex = stepCount;
	}
}

int Slider::getSelectedIndex() {
	return selectedIndex;
}

void Slider::mousePressed(MouseButton mouseButton, Vector2& mousePosition) {
	if (mouseButton == LEFT) {
		isSliding = true;
		update(mousePosition);
	}
}

void Slider::mouseReleased(MouseButton mouseButton, Vector2& mousePosition) {
	if (mouseButton == LEFT) {
		isSliding = false;
	}
}

void Slider::mouseMoved(Vector2& mousePosition) {
	if (isSliding) {
		update(mousePosition);
	}
}

void Slider::onHoverOff(Vector2& mousePosition) {
	isSliding = false;
}