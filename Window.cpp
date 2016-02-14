#include "Window.h"
#include "GUI.h"
#include "Vector2.h"
#include "ColourRGBA.h"

#include <iostream>

Window::Window(const std::string& id, Vector2 & position, Vector2& dimensions) : GUIElement(id, position, dimensions) {
	setDraggable(true);
	createCloseButton();
}

Window::Window(const std::string& id, Vector2& position, Vector2& dimensions, ColourRGBA& backgroundColour) : GUIElement(id, position, dimensions) {
	setDraggable(true);
	setColour(backgroundColour);
	createCloseButton();
}

Window::Window(const std::string& id, Vector2& position, Vector2& dimensions, const std::string& backgroundImage) : GUIElement(id, position, dimensions) {
	setDraggable(true);

	p_backgroundImage = new GUIImage(id + "_back", backgroundImage, dimensions);
	p_backgroundImage->setResponsive(false);
	attachChild(p_backgroundImage);

	createCloseButton();
}

void close(GUIElement& parent, Button& button) {
	if (parent.getParentElement() != nullptr) { // The window is a child of another element
		parent.getParentElement()->detachChild(&parent);
	}
	else {
		GUI::get().getState()->detachChild(&parent);
	}
}

void Window::createCloseButton() {
	p_closeButton = new Button(getID() + "_closeButton", Vector2(getDimensions().getX() - 30, getDimensions().getY() - 30), "images/scroll/SCROLL.tga");
	p_closeButton->assignImages("images/scroll/SCROLL.tga", "images/scroll/SCROLL_HOVER.tga", "images/scroll/SCROLL_PRESSED.tga");
	p_closeButton->setOnPress(close);

	GUIImage* crossImage = new GUIImage(getID() + "_closeButton_cross", "images/CROSS.tga");
	crossImage->setResponsive(false);

	p_closeButton->attachChild(crossImage);
	attachChild(p_closeButton);
}

void Window::render() {
	if (p_backgroundImage != nullptr) { // No need to render the base quad
		return;
	}
	glBegin(GL_TRIANGLE_STRIP);

	for (int i = 0; i != 4; i++) {
		if (hasGradiant()) {
			if ((p_gradiant->type == TO_BOTTOM && i < 2)
			 || (p_gradiant->type == TO_TOP && i > 1)
			 || (p_gradiant->type == TO_LEFT && (i == 3 || i == 1))
			 || (p_gradiant->type == TO_RIGHT && (i == 2 || i == 0))) {
				glColor4f(p_gradiant->colour.getRed(), p_gradiant->colour.getGreen(), p_gradiant->colour.getBlue(), getAlpha());
			}
			else {
				glColor4f(p_colour->getRed(), p_colour->getGreen(), p_colour->getBlue(), getAlpha());
			}
		}
		else {
			glColor4f(p_colour->getRed(), p_colour->getGreen(), p_colour->getBlue(), getAlpha());
		}
		glVertex2f(p_quad->getVertex(i).getX(), p_quad->getVertex(i).getY());
	}
	glEnd();
}

void Window::resize(float width, float height) {
	if (p_backgroundImage != nullptr) {
		p_backgroundImage->resize(width, height);
	}
	p_closeButton->setLocalTranslation(Vector2(getDimensions().getX() - 30, getDimensions().getY() - 30));

	GUIElement::resize(width, height);
}

