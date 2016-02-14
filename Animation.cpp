#include "Animation.h"

#include <stdlib.h>
#include <iostream>

Animation::Animation(const std::string& id, const std::string& imageFile, int cellSize, int frameCount) : GUIImage(id, imageFile, Vector2((float) cellSize, (float) cellSize)) {
	p_frameCounts = new int {frameCount};
	tilePercentage.set((float) cellSize / getTexWidth(), (float) cellSize / getTexHeight());
}

Animation::Animation(const std::string& id, const std::string& imageFile, int cellSize, int* frameCounts) : GUIImage(id, imageFile, Vector2((float) cellSize, (float) cellSize)) {
	p_frameCounts = frameCounts;
	tilePercentage.set((float) cellSize / getTexWidth(), (float) cellSize / getTexHeight());
}

Animation::~Animation() {
	delete[] p_frameCounts;
	GUIElement::~GUIElement();
}

void Animation::render() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_TRIANGLE_STRIP);

	// Used to apply a tint and/or alpha.
	glColor4f(p_colour->getRed(), p_colour->getGreen(), p_colour->getBlue(), getAlpha());

	for (int i = 0; i != 4; i++) {
		switch(i) {
			case 0:
				glTexCoord2f(tilePercentage.getX() * currentFrame, 1.0f - ((tilePercentage.getY() * currentAnimation) + tilePercentage.getY()));
			break;

			case 1:
				glTexCoord2f((tilePercentage.getX() * currentFrame) + tilePercentage.getX(), 1.0f - ((tilePercentage.getY() * currentAnimation) + tilePercentage.getY()));
			break;

			case 2:
				glTexCoord2f(tilePercentage.getX() * currentFrame, 1.0f - (tilePercentage.getY() * currentAnimation));
			break;

			case 3:
				glTexCoord2f((tilePercentage.getX() * currentFrame) + tilePercentage.getX(), 1.0f - (tilePercentage.getY() * currentAnimation));
			break;
		}
		glVertex2f(p_quad->getVertex(i).getX(), p_quad->getVertex(i).getY());
	}

	glEnd();
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);

	if (GetTickCount() - lastChangeTick >= delayMS) {
		if (!unordered) {
			if (currentFrame < p_frameCounts[currentAnimation] - 1) {
				currentFrame++;
			}
			else if (loops) {
				currentFrame = 0;
			}
		}
		else {
			currentFrame = rand() % p_frameCounts[currentAnimation];
		}
		lastChangeTick = GetTickCount();
	}
}

void Animation::setAnimation(int currentAnimation) {
	currentFrame = 0;
	this->currentAnimation = currentAnimation;
}

void Animation::setDelay(int delayMS) {
	this->delayMS = delayMS;
}

void Animation::setLooping(bool loops) {
	this->loops = loops;
}

void Animation::setUnordered(bool unordered) {
	this->unordered = unordered;
}
