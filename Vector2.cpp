#include "Vector2.h"

#include <sstream>

Vector2::Vector2() {
	set(0, 0);
}

Vector2::Vector2(float x, float y) {
	set(x, y);
}

Vector2::Vector2(Vector2& copy) {
	set(copy.p_elements[0], copy.p_elements[1]);
}

Vector2::~Vector2() {
	delete[] p_elements;
}

bool Vector2::operator!=(Vector2& toCheck) {
	return p_elements[0] != toCheck.p_elements[0]
		|| p_elements[1] != toCheck.p_elements[1];
}

bool Vector2::operator==(Vector2& toCheck) {
	return p_elements[0] == toCheck.p_elements[0]
		&& p_elements[1] == toCheck.p_elements[1];
}

Vector2 Vector2::operator+(Vector2& toAdd) {
	Vector2 result = Vector2(*this);

	for (int i = 0; i != 2; i++) {
		result.p_elements[i] += toAdd.p_elements[i];
	}
	return result;
}

Vector2 Vector2::operator-(Vector2& toSubtract) {
	Vector2 result = Vector2(*this);

	for (int i = 0; i != 2; i++) {
		result.p_elements[i] -= toSubtract.p_elements[i];
	}
	return result;
}

Vector2 Vector2::interpolate(Vector2& destinationVector, float percentage) {
	if (*this == destinationVector) {
		return *this;
	}
	return *this * (1 - percentage) + (destinationVector * percentage);
}

Vector2 Vector2::operator*(float scalar) {
	Vector2 result = Vector2(*this);

	for (int i = 0; i != 2; i++) {
		result.p_elements[i] *= scalar;
	}
	return result;
}

Vector2 Vector2::operator*(Vector2& toMultiply) {
	Vector2 result = Vector2(*this);

	for (int i = 0; i != 2; i++) {
		result.p_elements[i] *= toMultiply.p_elements[i];
	}
	return result;
}

Vector2 Vector2::operator/(Vector2& toDivide) {
	Vector2 result = Vector2(*this);

	for (int i = 0; i != 2; i++) {
		if (result.p_elements[i] != 0 && toDivide.p_elements[i] != 0) {
			result.p_elements[i] /= toDivide.p_elements[i];
		}
	}
	return result;
}

Vector2 Vector2::operator/(float scalar) {
	if (scalar == 0) {
		return Vector2();
	}

	Vector2 result = Vector2(*this);

	for (int i = 0; i != 2; i++) {
		if (result.p_elements[i] != 0) {
			result.p_elements[i] /= scalar;
		}
	}
	return result;
}

Vector2& Vector2::set(Vector2& setTo) {
	p_elements[0] = setTo.getX();
	p_elements[1] = setTo.getY();
	return *this;
}

Vector2& Vector2::set(float x, float y) {
	p_elements[0] = x;
	p_elements[1] = y;
	return *this;
}

Vector2& Vector2::set(int x, int y) {
	p_elements[0] = (float) x;
	p_elements[1] = (float) y;
	return *this;
}

Vector2& Vector2::setX(float x) {
	p_elements[0] = x;
	return *this;
}

Vector2& Vector2::setY(float y) {
	p_elements[1] = y;
	return *this;
}

float Vector2::getX() {
	return p_elements[0];
}

float Vector2::getY() {
	return p_elements[1];
}

bool Vector2::equals(float x, float y) {
	return p_elements[0] == x && p_elements[1] == y;
}

std::string Vector2::toString() {
	std::ostringstream stringStream;

	stringStream << "(" << p_elements[0] << ", " << p_elements[1] << ")";

	return stringStream.str();
}