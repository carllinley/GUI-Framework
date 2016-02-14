#include "Button.h"
#include "iostream"
#include "GUI.h"

// Labels aren't attached as children to make life easier - they're always on top.

Button::Button(const std::string& id, Vector2& localTranslation) : GUIElement(id, localTranslation) {
}

Button::Button(const std::string& id, Vector2& localTranslation, Vector2& dimensions) : GUIElement(id, localTranslation, dimensions) {
}

Button::Button(const std::string& id, Vector2& localTranslation, const std::string& imagePath) : GUIElement(id, localTranslation) {
	setImage(IDLE, new GUIImage(id + "_image", imagePath));

	Texture* texture = GUI::get().getTexture(imagePath);
	resize((float) texture->width, (float) texture->height);
}

Button::Button(const std::string& id, Vector2& localTranslation, Vector2& dimensions, const std::string& label) : GUIElement(id, localTranslation, dimensions) {
	p_label = new Label(id + "_label", label.c_str(), 10, Vector2(10, 29));
}

Button::Button(const std::string& id, Vector2& localTranslation, Vector2& dimensions, Label& label) : GUIElement(id, localTranslation, dimensions) {
	p_label = &label;
}

Button::Button(const std::string& id, Vector2& localTranslation, Vector2& dimensions, GUIImage& image) : GUIElement(id, localTranslation, dimensions) {
	setImage(IDLE, &image);
	updateImage(IDLE);
}

Button::~Button() {
	delete p_label;
	delete p_hoverColour;
	GUIElement::~GUIElement();
}

void Button::render() {
	if (images[0] == nullptr) { // If the button has an image, we'll just cover this.
		glBegin(GL_TRIANGLE_STRIP);

		for (int i = 0; i != 4; i++) {
			glColor4f(p_colour->getRed(), p_colour->getGreen(), p_colour->getBlue(), getAlpha());
			glVertex2f(p_quad->getVertex(i).getX(), p_quad->getVertex(i).getY());
		}
		glEnd();
	}
}

void Button::renderPostDescendants() {
	if (p_label != nullptr) {
		p_label->render(); // Always on top - would be better done via bringToFront
	}
}

void Button::resize(float x, float y) {
	for (int i = 0; i != 3; i++) {
		if (images[i] != nullptr) {
			images[i]->resize(x, y);
		}
	}
	GUIElement::resize(x, y);
}

void Button::assignImages(const std::string& idleImage, const std::string& hoverImage, const std::string& pressedImage) {
	setImage(IDLE, new GUIImage(elementID + "_idle", idleImage));
	setImage(HOVER, new GUIImage(elementID + "_hover", hoverImage));
	setImage(PRESS, new GUIImage(elementID + "_press", pressedImage));
	updateImage(IDLE);
}

void Button::setOnPress(void (*onPress) (GUIElement& parentElement, Button& thisButton)) {
	p_func_overrideOnPress = onPress;
}

void Button::updateImage(ButtonEvent state) {
	currentState = state;

	if (currentState != IDLE && images[currentState] == nullptr && images[IDLE] != nullptr) {
		images[IDLE]->setVisible(true);
		return;
	}

	for (int i = 0; i != 3; i++) {
		if (images[i] != nullptr) {
			images[i]->setVisible(currentState == i); // Bit cheeky
		}
	}
}

void Button::setHoverForegroundColour(ColourRGBA& colour) {
	if (p_hoverColour != nullptr) {
		p_hoverColour->set(colour);
	}
	else {
		p_hoverColour = new ColourRGBA(colour);
	}
}

void Button::setImage(ButtonEvent type, GUIImage* image) {
	if (images[type] != nullptr) {
		detachChild(images[type]);
	}
	images[type] = image;

	if (type != IDLE) { // Idle is visible by default
		image->setVisible(false);
	}
	else {
		resize(image->getDimensions().getX(), image->getDimensions().getY());
	}
	image->setResponsive(false);
	attachChild(image);
}

void Button::setLabel(Label* label) {
	if (p_label != nullptr) {
		delete p_label;
	}
	p_label = label;
}

Label* Button::getLabel() {
	return p_label;
}

void Button::mousePressed(MouseButton mouseButton, Vector2& mousePosition) {
	GUIElement::mousePressed(mouseButton, mousePosition);
	updateImage(PRESS);

	if (p_func_overrideOnPress != nullptr) {
		p_func_overrideOnPress(*p_parent, *this);
	}
	else if (GUI::get().getState() != nullptr) {
		GUI::get().getState()->buttonPressed(*this);
	}
}

void Button::mouseReleased(MouseButton mouseButton, Vector2& mousePosition) {
	GUIElement::mouseReleased(mouseButton, mousePosition);
	updateImage(HOVER);
}

void Button::onHover(Vector2& mousePosition) {
	GUIElement::onHover(mousePosition);
	updateImage(HOVER);

	if (p_label != nullptr && p_hoverColour != nullptr) {
		if (p_standardColour != nullptr) {
			p_standardColour->set(p_label->getColour());
		}
		else {
			p_standardColour = new ColourRGBA(p_label->getColour());
		}
		p_label->setColour(*p_hoverColour);
	}
}

void Button::onHoverOff(Vector2& mousePosition) {
	GUIElement::onHoverOff(mousePosition);
	updateImage(IDLE);

	if (p_label != nullptr && p_standardColour != nullptr) {
		p_label->setColour(*p_standardColour);
	}
}