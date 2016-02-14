#include "Label.h"
#include "ColourRGBA.h"
#include "GUI.h"

#include <iostream>
#include <fstream>

Label::Label(const std::string& id, const std::string& text, Vector2& position) : GUIElement(id, position) {
	setText(text);
	setFont("fonts/franklin_gothic.glf");
	setResponsive(false);
}

Label::Label(const std::string& id, const std::string& text, int fontSize, Vector2& position) : GUIElement(id, position) {
	setText(text);
	setFontSize(fontSize);
	setFont("fonts/franklin_gothic.glf");
	setResponsive(false);
}

Label::Label(const std::string& id, const std::string& text, int fontSize, ColourRGBA& colour, Vector2& position) : GUIElement(id, position) {
	setText(text);
	setFontSize(fontSize);
	setFont("fonts/franklin_gothic.glf");
	setColour(colour);
	setResponsive(false);
}

Label::~Label() {
	delete[] p_text;
	GUIElement::~GUIElement();
}

void Label::render() {
	p_GLFont->Begin();

	const char* c_str = text.c_str();

	if (dropShadow) {
		glColor4f(0, 0, 0, p_colour->getAlpha());
		p_GLFont->RenderText(c_str, getLocalTranslation().getX() + 2, getLocalTranslation().getY() - 2, 0, size);
	}
	glColor4f(p_colour->getRed(), p_colour->getGreen(), p_colour->getBlue(), getAlpha());
	p_GLFont->RenderText(c_str, getLocalTranslation().getX(), getLocalTranslation().getY(), 0, size);
	p_GLFont->End();
}

void Label::setShadowed(bool dropShadow) {
	this->dropShadow = dropShadow;
}

void Label::setFont(const std::string& filePath) {
	p_GLFont = GUI::get().getFont(filePath);
}

void Label::setFontSize(int size) {
	this->size = (float) size;
}

void Label::setText(const std::string& text) {
	this->text = text;
}

std::string Label::getText() {
	return text;
}

int Label::getFontSize() {
	return (int) size;
}