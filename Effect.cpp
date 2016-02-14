#include "Effect.h"
#include "GUIElement.h"

#include <iostream>

// Stores all effect classes

Effect::Effect(float speed) {
	this->speed = speed;
}

Effect::~Effect() {
}

void Effect::start() {
}

void Effect::onCompletion() {
}

void Effect::setSpeed(float speed) {
	this->speed = (speed != 0 ? speed / 100 : 0);
}

void Effect::setElement(GUIElement* element) {
	p_element = element;
	initialised = true;
}

bool Effect::hasInitialised() {
	return initialised;
}

/*
 ShrinkOut
 */

ShrinkOut::ShrinkOut(float shrinkSpeed) : Effect(shrinkSpeed != 0 ? shrinkSpeed / 100 : 0) {
}

void ShrinkOut::start() {
	originScale = p_element->getScale();
	p_scale = p_element->p_scale();
}

void ShrinkOut::update(float timePerFrame) {
	float next = *p_scale - (speed * timePerFrame);

	if (next <= 0) {
		isComplete = true;
	}
	else {
		*p_scale = next;
	}
}

void ShrinkOut::onCompletion() {
	p_element->setVisible(false);
	*p_scale = originScale;
}

/*
 Grow in
 */

GrowIn::GrowIn(float growSpeed) : Effect(growSpeed != 0 ? growSpeed / 100 : 0) {
}

void GrowIn::start() {
	originScale = p_element->getScale();
	p_scale = p_element->p_scale();
	*p_scale = 0;
	p_element->setVisible(true);
}

void GrowIn::update(float timePerFrame) {
	float next = *p_scale + (speed * timePerFrame);

	if (next >= originScale) {
		isComplete = true;
	}
	else {
		*p_scale = next;
	}
}

void GrowIn::onCompletion() {
	p_element->setVisible(true);
	*p_scale = originScale;
}

/* Rotation */

Rotate::Rotate(float rotationSpeed) : Effect(rotationSpeed) {
}

Rotate::Rotate(float rotationSpeed, RotationDirection rotationDirection) : Effect(rotationSpeed) {
	direction = rotationDirection;
}

Rotate::Rotate(float rotationSpeed, RotationDirection rotationDirection, bool returnToOrigin) : Effect(rotationSpeed) {
	direction = rotationDirection;
	returnOrigin = returnToOrigin;
}

void Rotate::start() {
	originAngle = p_element->getLocalRotation();
	currentAngle = originAngle + speed;
}

void Rotate::update(float timePerFrame) {
	float nextAngle = currentAngle + (direction == CLOCKWISE ? -(speed * timePerFrame) : (speed * timePerFrame));

	if (nextAngle > 360 || nextAngle < 0) {
		nextAngle = (direction == CLOCKWISE ? 360 - nextAngle : nextAngle - 360);
	}
	// TODO support for revolution counting

	currentAngle = nextAngle;
	p_element->setLocalRotation(currentAngle);
}

void Rotate::onCompletion() {
	if (returnOrigin) {
		p_element->setLocalRotation(originAngle);
	}
}

void Rotate::setSpeed(float speed) {
	this->speed = speed;
}

/*
 FadeIn
 */

FadeIn::FadeIn(float fadeSpeed) : Effect(fadeSpeed != 0 ? fadeSpeed / 100 : 0) {
}

void FadeIn::start() {
	p_element->getColour().setAlpha(0);
	p_element->setVisible(true);
	p_element->descendAlpha(&currentAlpha);
}

void FadeIn::update(float timePerFrame) {
	currentAlpha += (speed * timePerFrame);

	if (currentAlpha >= 1.0f) {
		isComplete = true;
		p_element->getColour().setAlpha(1);
	}
}

void FadeIn::onCompletion() {
	p_element->descendAlpha(nullptr);
}

/*
FadeOut
*/

FadeOut::FadeOut(float fadeSpeed) : Effect(fadeSpeed != 0 ? fadeSpeed / 100 : 0) {
}

void FadeOut::start() {
	currentAlpha = p_element->getAlpha();

	p_element->setVisible(true);
	p_element->descendAlpha(&currentAlpha);
}

void FadeOut::update(float timePerFrame) {
	currentAlpha -= (speed * timePerFrame);

	if (currentAlpha <= 0.0f) {
		isComplete = true;
	}
}

void FadeOut::onCompletion() {
	p_element->getColour().setAlpha(0);
	// p_element->setVisible(false);
}

/*
 Pulse
 */

Pulse::Pulse(float fadeSpeed) : Effect(fadeSpeed != 0 ? fadeSpeed / 100 : 0) {
}

Pulse::Pulse(float fadeSpeed, float fadeIntensity) : Effect(fadeSpeed != 0 ? fadeSpeed / 100 : 0) {
	intensity = fadeIntensity;
}

void Pulse::start() {
	currentAlpha = p_element->getAlpha();

	p_element->setVisible(true);
	p_element->descendAlpha(&currentAlpha);
}

void Pulse::update(float timePerFrame) {
	if (direction) {
		currentAlpha -= (speed * timePerFrame);

		if (currentAlpha <= intensity) {
			currentAlpha = intensity;
			direction = !direction;
		}
	}
	else {
		currentAlpha += (speed * timePerFrame);

		if (currentAlpha >= 1.0f) {
			currentAlpha = 1.0f;
			direction = !direction;
		}
	}
}

void Pulse::onCompletion() {
	p_element->descendAlpha(nullptr);
}

/*
 Flash
 */

Flash::Flash(float flashSpeed) : Effect(flashSpeed) {
}

Flash::Flash(float flashSpeed, int numberOfTimes) : Effect(flashSpeed) {
	this->times = numberOfTimes;
}

void Flash::start() {
	p_element->setVisible(true);
}

void Flash::update(float timePerFrame) {
	progress += (speed * timePerFrame);

	if (progress >= 5.0f) {
		progress = 0;
		p_element->setVisible(!p_element->isVisible());
		flashCount++;

		if (times != 0 && flashCount >= times) {
			isComplete = true;
		}
	}
}

void Flash::onCompletion() {
	p_element->setVisible(true);
}

void Flash::setSpeed(float speed) {
	this->speed = speed;
}

/*
 ChangeColour - changes the element to the specified colour, and back again when the effect finishes.
 */

ChangeColour::ChangeColour(const ColourRGBA& colour) : Effect(0) {
	newColour.set(colour);
}

ChangeColour::ChangeColour(ColourRGBA& colour) : Effect(0) {
	newColour.set(colour);
}

void ChangeColour::start() {
	originalColour.set(p_element->getColour());
	p_element->setColour(newColour);
}

void ChangeColour::update(float timePerFrame) {
}

void ChangeColour::onCompletion() {
	p_element->setColour(originalColour);
}
