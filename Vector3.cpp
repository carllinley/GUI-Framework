#include "Vector3.h"

Vector3::Vector3() : Vector(3) {
	set(0, 0, 0);
}

Vector3::Vector3(float x, float y, float z) : Vector(3) {
	set(x, y, z);
}

Vector3::Vector3(Vector& conversion) : Vector(3) {
	set(conversion.getElement(0), conversion.getElement(1), conversion.getElement(2));
}

void Vector3::set(float x, float y, float z) {
	p_elements[0] = x;
	p_elements[1] = y;
	p_elements[2] = z;
}

void Vector3::setX(float x) {
	p_elements[0] = x;
}

void Vector3::setY(float y) {
	p_elements[1] = y;
}

void Vector3::setZ(float z) {
	p_elements[2] = z;
}

float Vector3::getX() {
	return p_elements[0];
}

float Vector3::getY() {
	return p_elements[1];
}

float Vector3::getZ() {
	return p_elements[2];
}