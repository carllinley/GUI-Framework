#include "ColourRGBA.h"
#include <iostream>

const ColourRGBA ColourRGBA::WHITE = ColourRGBA(1, 1, 1, 1),
				 ColourRGBA::BLACK = ColourRGBA(0, 0, 0, 1),
				 ColourRGBA::RED = ColourRGBA(1, 0, 0, 1),
				 ColourRGBA::GREEN = ColourRGBA(0, 1, 0, 1),
				 ColourRGBA::BLUE = ColourRGBA(0, 0, 1, 1),
				 ColourRGBA::CYAN = ColourRGBA(0, 1, 1, 1),
				 ColourRGBA::MAGENTA = ColourRGBA(1, 0, 1, 1),
				 ColourRGBA::YELLOW = ColourRGBA(1, 1, 0, 1);

ColourRGBA::ColourRGBA(float red, float green, float blue, float alpha) {
	p_values = new float[4] {red, green, blue, alpha};
}

ColourRGBA::ColourRGBA() {
	p_values = new float[4] {0, 0, 0, 1};
}

ColourRGBA::ColourRGBA(const ColourRGBA& toCopy) {
	p_values = new float[4] {toCopy.p_values[0], toCopy.p_values[1], toCopy.p_values[2], toCopy.p_values[3]};
}

ColourRGBA::~ColourRGBA() {
	delete[] p_values;
}

ColourRGBA& ColourRGBA::operator=(ColourRGBA& toAssign) {
	set(toAssign);
	return *this;
}

ColourRGBA& ColourRGBA::operator-(ColourRGBA& toSubtract) {
	for (int i = 0; i != 4; i++) {
		p_values[i] -= toSubtract.p_values[i];
	}
	normalise();
	return *this;
}

ColourRGBA& ColourRGBA::operator*(ColourRGBA& toMult) {
	for (int i = 0; i != 4; i++) {
		p_values[i] *= toMult.p_values[i];
	}
	normalise();
	return *this;
}

ColourRGBA& ColourRGBA::operator*(float scalar) {
	for (int i = 0; i != 4; i++) {
		p_values[i] *= scalar;
	}
	normalise();
	return *this;
}

ColourRGBA & ColourRGBA::operator/(float scalar) {
	if (scalar == 0) {
		return *this;
	}

	for (int i = 0; i != 4; i++) {
		if (p_values[i] != 0) {
			p_values[i] /= scalar;
		}
	}
	normalise();
	return *this;
}

void ColourRGBA::set(const ColourRGBA& colour) {
	for (int i = 0; i != 4; i++) {
		p_values[i] = colour.p_values[i];
	}
}

void ColourRGBA::setRed(float red) {
	this->p_values[0] = red;
}

void ColourRGBA::setGreen(float green) {
	this->p_values[1] = green;
}

void ColourRGBA::setBlue(float blue) {
	this->p_values[2] = blue;
}

void ColourRGBA::setAlpha(float alpha) {
	this->p_values[3] = alpha;
}

float ColourRGBA::getRed() {
	return p_values[0];
}

float ColourRGBA::getGreen() {
	return p_values[1];
}
		
float ColourRGBA::getBlue() {
	return p_values[2];
}

float ColourRGBA::getAlpha() {
	return p_values[3];
}

void ColourRGBA::normalise() {
	for (int i = 0; i != 4; i++) {
		if (p_values[i] < 0) {
			p_values[i] = 0;
		}
		else if (p_values[i] > 1) {
			p_values[i] = 1;
		}
	}
}
