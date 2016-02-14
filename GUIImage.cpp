#include "GUIImage.h"
#include "GUI.h"

#include <iostream>

GUIImage::GUIImage(const std::string id, std::string fileName) : GUIElement(id, Vector2(), Vector2()) {
	Texture* texture = GUI::get().getTexture(fileName);

	if (texture != nullptr) {
		textureID = GUI::get().getTexture(fileName)->glID;
		p_quad->resize(Vector2((float) GUI::get().getTexture(fileName)->width, (float) GUI::get().getTexture(fileName)->height));
	}
	this->fileName = fileName;
}

GUIImage::GUIImage(const std::string id, std::string fileName, Vector2& dimensions) : GUIElement(id, Vector2(), dimensions) {
	if (GUI::get().getTexture(fileName) != nullptr) {
		textureID = GUI::get().getTexture(fileName)->glID;
		this->fileName = fileName;
	}
}

void GUIImage::setTiling(int tilesX, int tilesY) {
	tiling.set((float) tilesX, (float) tilesY);
}

void GUIImage::render() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_TRIANGLE_STRIP);

	// Used to apply a tint and/or alpha.
	glColor4f(p_colour->getRed(), p_colour->getGreen(), p_colour->getBlue(), getAlpha());

	for (int i = 0; i != 4; i++) {
		if (i == 0) {
			glTexCoord2f(0.0, 0.0);
		}
		if (i == 1) {
			glTexCoord2f(tiling.getX(), 0.0);
		}
		if (i == 2) {
			glTexCoord2f(0.0, tiling.getY());
		}
		if (i == 3) {
			glTexCoord2f(tiling.getX(), tiling.getY());
		}
		glVertex2f(p_quad->getVertex(i).getX(), p_quad->getVertex(i).getY());
	}

	glEnd();
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);
}

float GUIImage::getTexWidth() {
	return (float) GUI::get().getTexture(fileName)->width;
}

float GUIImage::getTexHeight() {
	return (float) GUI::get().getTexture(fileName)->height;
}
