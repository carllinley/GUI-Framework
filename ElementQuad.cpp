#include "ElementQuad.h"
#include <iostream>

// const float ElementQuad::DEG_TO_RADS = 3.141592653f / 180.0f; // OpenGL uses degrees

ElementQuad::ElementQuad(Vector2& position, Vector2& dimensions) {
	init(new Vector2[4] {Vector2(0, 0),
						 Vector2(dimensions.getX(), 0),
						 Vector2(0, dimensions.getY()),
						 Vector2(dimensions.getX(), dimensions.getY())});
	
	translate(position);
	this->p_halfDimensions = new Vector2(dimensions / 2);
}

ElementQuad::~ElementQuad() {
}

void ElementQuad::init(Vector2* p_vertexList) {
	this->p_vertexList = p_vertexList;
	p_translation = new Vector2();
	_rotation = 0;
	_scale = 1;
}

void ElementQuad::resize(Vector2& dimensions) {
	p_vertexList[0].set(0.0f, 0.0f);
	p_vertexList[1].set(dimensions.getX(), 0.0f);
	p_vertexList[2].set(0.0f, dimensions.getY());
	p_vertexList[3].set(dimensions.getX(), dimensions.getY());

	p_halfDimensions->set(dimensions / 2); // What in the piss is going on here?
}

float ElementQuad::getHalfX() {
	return p_halfDimensions->getX();
}

float ElementQuad::getHalfY() {
	return p_halfDimensions->getY();
}

Vector2* ElementQuad::getVertices() {
	return p_vertexList;
}

void ElementQuad::translate(Vector2& translation) {
	p_translation->set(translation);
}

void ElementQuad::scale(float scale) {
	_scale = scale;
}

void ElementQuad::rotate_Degrees(float angleDegrees) {
	_rotation = angleDegrees; // angleDegrees * DEG_TO_RADS;
}

Vector2& ElementQuad::getVertex(int i) {
	return p_vertexList[i];
}

Vector2& ElementQuad::getTranslation() {
	return *p_translation;
}

Vector2* ElementQuad::translation() {
	return p_translation;
}

float ElementQuad::getScale() {
	return _scale;
}

float ElementQuad::getRotaton() {
	return _rotation;
}