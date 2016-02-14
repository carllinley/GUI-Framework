#include "GUIElement.h"
#include "GUI.h"
#include "Effect.h"

#include <iostream>
#include <string>

GUIElement::GUIElement(const std::string& id, Vector2& position) {
	init(id, position, Vector2());
}

GUIElement::GUIElement(const std::string& id, Vector2& position, Vector2& dimensions) {
	init(id, position, dimensions);
}

GUIElement::~GUIElement() {
	GUI::get().cleanup(this);

	if (effects.size() != 0) {
		for (auto kv : effects) { // Free any effects
			delete kv.second;
		}
	}
	delete p_quad;
	delete p_dragData;
	delete p_resizeData;
	delete p_colour;
}

bool GUIElement::operator==(const char* id) {
	return elementID == id;
}

bool GUIElement::operator==(const std::string& id) {
	return elementID == id;
}

bool GUIElement::operator!=(const std::string& id) {
	return elementID == id;
}

void GUIElement::bringToFront() {
	GUIElement* currentAncestor = p_parent;
	GUIElement* currentAncestorChild = this;

	while (currentAncestor != nullptr) {
		currentAncestor->children.erase(std::find(currentAncestor->children.begin(), currentAncestor->children.end(), currentAncestorChild));
		currentAncestor->children.insert(currentAncestor->children.end(), currentAncestorChild);

		currentAncestorChild = currentAncestor;
		currentAncestor = currentAncestor->p_parent;
	}

	GUIState* state = GUI::get().getState();
	state->getChildren().erase(std::find(state->getChildren().begin(), state->getChildren().end(), currentAncestorChild));
	state->getChildren().insert(state->getChildren().end(), currentAncestorChild);
}

Effect* GUIElement::getEffect(EffectTrigger trigger) {
	return effects[trigger];
}

void GUIElement::init(const std::string& id, Vector2& position, Vector2& dimensions) {
	elementID = id;
	p_quad = new ElementQuad(position, dimensions);
	children = std::vector<GUIElement*>();

	setVisible(true);
}

void GUIElement::update(float timePerFrame) {
	if (p_currentEffect != nullptr) {
		if (!p_currentEffect->isComplete) {
			p_currentEffect->update(timePerFrame);
		}
		else {
			p_currentEffect->onCompletion();

			if (effects[TRIG_DETACHED] != nullptr && p_currentEffect == effects[TRIG_DETACHED]) { // Detach effect complete, detach the element
				getParent()->detachChild(this);
			}
			else {
				playEffect(nullptr);
			}
		}
	}
}

void GUIElement::resize(float width, float height) {
	p_quad->resize(Vector2(width, height));
}

void GUIElement::render() {
}

void GUIElement::renderPostDescendants() {
}

// Base GUIElement generally doesn't care about these - they exist for subclasses.

void GUIElement::onFocus(Vector2& mousePosition) {}
void GUIElement::onLostFocus(Vector2& mousePosition) {}
void GUIElement::charPressed(char keyCharacter) {}
void GUIElement::charReleased(char keyCharacter) {}
void GUIElement::keyPressed(int keyID) {}
void GUIElement::keyReleased(int keyID) {}
void GUIElement::mouseWheelUp(Vector2 & mousePosition) {}
void GUIElement::mouseWheelDown(Vector2 & mousePosition) {}

void GUIElement::mousePressed(MouseButton mouseButton, Vector2& mousePosition) {
	pressed = true;

	switch (mouseButton) {
		case LEFT:
			if (isDraggable()) {
				p_dragData->lastUpdate.set(mousePosition);
				p_dragData->isDragging = true;
			}
		break;

		case RIGHT:
			if (isResizable()) { // TODO buggy!
				Vector2* worldTranslation = getWorldTranslation();
				Vector2 localCoordinates = *worldTranslation + mousePosition - (GUI::get().getDimensions() / 2.0f);

				if ((localCoordinates.getX() < 10 || localCoordinates.getX() > getDimensions().getX() - 10)
					&& (localCoordinates.getY() < 10 || localCoordinates.getX() > getDimensions().getY() - 10)) {
					p_resizeData->lastUpdate.set(mousePosition);
					p_resizeData->isDragging = true;
				}

				delete worldTranslation;
			}
		break;

		case MIDDLE: // Debug purposes
			Vector2* worldTranslation = getWorldTranslation();
			std::cout << "Local translation: " << getLocalTranslation().toString() << " / World translation: " << worldTranslation->toString() << std::endl;
			delete worldTranslation;
		break;
	}
}

void GUIElement::mouseReleased(MouseButton mouseButton, Vector2& mousePosition) {
	pressed = false;

	switch (mouseButton) {
		case LEFT:
			if (isDraggable()) {
				p_dragData->isDragging = false;
			}
		break;

		case RIGHT:
			if (isResizable()) {
				p_resizeData->isDragging = false;
			}
		break;
	}
}

void GUIElement::mouseMoved(Vector2& mousePosition) {
	if (isResizable() && p_resizeData->isDragging) {
		Vector2 newSize = getDimensions() - (Vector2(mousePosition) - p_dragData->lastUpdate);

		p_dragData->lastUpdate.set(mousePosition);
		resize(newSize.getX(), newSize.getY());
	}
	else if (isDraggable() && p_dragData->isDragging) {
		Vector2 dragDistance = Vector2(mousePosition) - p_dragData->lastUpdate;

		p_dragData->lastUpdate.set(mousePosition);
		translate(dragDistance.getX(), -dragDistance.getY());
	}
}

void GUIElement::onHover(Vector2& mousePosition) {
	if (effects[TRIG_HOVER] != nullptr) {
		playEffect(effects[TRIG_HOVER]);
	}
}

void GUIElement::onHoverOff(Vector2& mousePosition) {
	pressed = false;

	if (isResizable()) {
		p_resizeData->isDragging = false;
	}
	if (isDraggable()) {
		p_dragData->isDragging = false;
	}

	if (effects[TRIG_HOVER_OFF] != nullptr) {
		playEffect(effects[TRIG_HOVER_OFF]);
	}
	else if (effects[TRIG_HOVER] != nullptr && p_currentEffect == effects[TRIG_HOVER]) {
		resetEffects();
	}
}

Vector2 GUIElement::getDimensions() {
	return Vector2(p_quad->getHalfX() * 2, p_quad->getHalfY() * 2);
}

std::string* GUIElement::getToolTip() {
	return p_tooltip;
}

void GUIElement::addEffect(EffectTrigger trigger, Effect* effect) {
	if (effect != nullptr) {
		effect->setElement(this);
	}
	effects[trigger] = effect;

	if (trigger == GENERIC) {
		playEffect(effect);
	}
}

void GUIElement::playEffect(Effect* effect) {
	if (p_currentEffect == effect) {
		return;
	}

	if (p_currentEffect != nullptr) {
		bool setEffect = false;

		for (int i = GENERIC; i != TRIG_PRESSED; i++) {
			EffectTrigger trigger = static_cast<EffectTrigger>(i);

			if (effects[trigger] == p_currentEffect) {
				setEffect = true;
				break;
			}
		}
		p_currentEffect->onCompletion();

		// Delete randomly played effects, but not assigned effects
		if (!setEffect) { 
			delete p_currentEffect;
		}
	}

	if (effect == nullptr) {
		p_currentEffect = nullptr;
		return;
	}

	if (!effect->hasInitialised()) {
		effect->setElement(this);
	}
	p_currentEffect = effect;
	p_currentEffect->start();
}

void GUIElement::resetEffects() {
	if (p_currentEffect != effects[GENERIC]) {
		playEffect(effects[GENERIC]); // If it's null, we reset - if not, we revert
	}
}

void GUIElement::setResponsive(bool isResponsive) {
	if (!pickable && isResponsive) {
		GUI::get().registerElement(this);
	}
	else if (pickable && !isResponsive) {
		GUI::get().unregisterElement(this);
	}
	pickable = isResponsive;
}

void GUIElement::setColour(const ColourRGBA& colour) { // Less ugly than "set(ColourRGBA(ColourRGBA::XXX))"
	p_colour->set(ColourRGBA(colour));

	for (int i = 0; i != children.size(); i++) {
		children[i]->setColour(colour);
	}
}

void GUIElement::setColour(ColourRGBA& colourRGBA) {
	p_colour->set(colourRGBA);
}

void GUIElement::setGradiant(GradiantType type) {
	setGradiant(type, 1.25f);
}

void GUIElement::setGradiant(GradiantType type, float intensity) {
	if (type == NO_GRADIANT || intensity == 0) {
		delete p_gradiant;
		return;
	}

	if (p_gradiant == nullptr) {
		p_gradiant = new Gradiant();
	}
	p_gradiant->type = type;
	p_gradiant->colour.set(ColourRGBA(getColour()) / intensity);
}

void GUIElement::setGradiant(GradiantType type, ColourRGBA& endColour) {
	if (type == NO_GRADIANT) {
		delete p_gradiant;
		return;
	}

	if (p_gradiant == nullptr) {
		p_gradiant = new Gradiant();
	}
	p_gradiant->type = type;
	p_gradiant->colour.set(endColour);
}

void GUIElement::setVisible(bool isVisible) {
	visible = isVisible;
}

void GUIElement::setToolTip(const std::string& tooltip) {
	if (p_tooltip != nullptr) {
		*p_tooltip = tooltip;
	}
	else {
		p_tooltip = new std::string(tooltip);
	}
}

void GUIElement::descendAlpha(float* p_alpha) {
	this->p_alpha = p_alpha;

	for (int i = 0; i != children.size(); i++) {
		children[i]->descendAlpha(p_alpha);
	}
}

void GUIElement::translate(float x, float y) {
	p_quad->translate(getLocalTranslation() + Vector2(x, y));
}

void GUIElement::setResizable(bool isResizable) {
	if (isResizable) {
		if (this->isResizable()) {
			return;
		}
		p_resizeData = new Draggable();
	}
	else {
		delete p_resizeData;
	}
}

void GUIElement::setDraggable(bool isDraggable) {
	if (!isDraggable) {
		delete p_dragData;
	}
	else {
		p_dragData = new Draggable();
	}
}

void GUIElement::setLocalTranslation(Vector2& translation) {
	p_quad->translate(translation);
}

void GUIElement::setLocalRotation(float rotation_degrees) {
	p_quad->rotate_Degrees(rotation_degrees);
}

void GUIElement::setScale(float scale) {
	p_quad->scale(scale);
}

void GUIElement::setPassFocusElement(GUIElement* p_element) {
	this->p_passFocus = p_element;
}

const std::string& GUIElement::getID() {
	try {
		return elementID;
	}
	catch (std::bad_alloc exception) {
		return "NO_ID_BAD_ALLOC";
	}
}

Vector2* GUIElement::getWorldTranslation() {
	Vector2* worldTranslation = new Vector2(getLocalTranslation());
	GUIElement* ancestor = p_parent;

	while (ancestor != nullptr) {
		worldTranslation->set(*worldTranslation + ancestor->getLocalTranslation());
		ancestor = ancestor->p_parent;
	}
	return worldTranslation;
}

Vector2& GUIElement::getLocalTranslation() {
	return p_quad->getTranslation();
}

float GUIElement::getLocalRotation() {
	return p_quad->getRotaton();
}

float GUIElement::getScale() {
	return p_quad->getScale();
}

ColourRGBA& GUIElement::getColour() {
	return *p_colour;
}

bool GUIElement::isDraggable() {
	return p_dragData != nullptr;
}

bool GUIElement::isResizable() {
	return p_resizeData != nullptr;
}

bool GUIElement::hasGradiant() {
	return p_gradiant != nullptr;
}

float* GUIElement::p_scale() {
	return &(p_quad->_scale);
}

GUIElement* GUIElement::getPassFocusElement() {
	return p_passFocus;
}

float GUIElement::getAlpha() {
	if (p_alpha != nullptr) {
		return *p_alpha;
	}
	return p_colour->getAlpha();
}

ElementQuad* GUIElement::getQuad() {
	return p_quad;
}

bool GUIElement::ancestorOf(GUIElement* checkAncestor) {
	GUIElement* p_currentAncestor = p_parent;

	while (p_currentAncestor != nullptr) {
		if (p_currentAncestor == checkAncestor) {
			return true;
		}
		p_currentAncestor = p_currentAncestor->p_parent;
	}
	return false;
}

bool GUIElement::isVisible() {
	return visible && getAlpha() > 0;
}

bool GUIElement::isResponsive() {
	return pickable && pickIndex != 0;
}

bool GUIElement::isPickable() {
	return pickable;
}

bool GUIElement::isPressed() {
	return pressed;
}
