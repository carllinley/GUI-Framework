#include "ScrollArea.h"
#include "GUI.h"
#include "GUIImage.h"

ScrollArea::ScrollArea(const std::string id, Vector2& localTranslation, Vector2& dimensions) : GUIElement(id, localTranslation, dimensions) {
	this->dimensions.set(dimensions);
	scrollable = true;
}

ScrollArea::~ScrollArea() {
	delete p_backgroundImage;
}

void ScrollArea::render() {
	Vector2* worldTranslation = getWorldTranslation();
	Vector2 screenPosition = *worldTranslation + (GUI::get().getDimensions() / 2.0f);
	screenPosition.setX(screenPosition.getX() - 10);
	screenPosition.setY(screenPosition.getY() - 20);

	glEnable(GL_SCISSOR_TEST);
	glScissor((GLint) screenPosition.getX(), (GLint) screenPosition.getY(), (GLsizei) dimensions.getX(), (GLsizei) dimensions.getY());
	
	if (p_backgroundImage == nullptr) {
		glBegin(GL_TRIANGLE_STRIP);
		glColor4f(p_colour->getRed(), p_colour->getGreen(), p_colour->getBlue(), getAlpha());

		for (int i = 0; i != 4; i++) {
			glVertex2f(p_quad->getVertex(i).getX(), p_quad->getVertex(i).getY());
		}
		glEnd();
	}
	else {
		p_backgroundImage->render();
	}
	delete worldTranslation;
}

void ScrollArea::setBackground(const std::string& filePath) {
	if (p_backgroundImage != nullptr) {
		delete p_backgroundImage;
	}
	p_backgroundImage = new GUIImage(getID() + "_back", filePath, getDimensions());
}