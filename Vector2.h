#pragma once

#include <string>

/*
 A simple two-dimensional vector.
 */
class Vector2 {
	public:
		Vector2();
		Vector2(float x, float y);
		Vector2(Vector2& copy);
		~Vector2();

		bool operator!=(Vector2& toCheck);
		bool operator==(Vector2& toCheck);
		Vector2 operator+(Vector2& toAdd);
		Vector2 operator*(Vector2& toMult);
		Vector2 operator*(float scalar);
		Vector2 operator/(Vector2& toDivide);
		Vector2 operator/(float scalar);
		Vector2 operator-(Vector2& toSubtract);

		/*
		 Returns a copy of this vector linearly interpolated with the specified vector using the specified percentage.
		 */
		Vector2 interpolate(Vector2& destinationVector, float percentage);

		/*
		 Sets x to the specified value and returns this vector.
		 */
		Vector2& setX(float x);

		/*
		 Sets y to the specified value and returns this vector.
		 */
		Vector2& setY(float y);

		Vector2& set(float x, float y);
		Vector2& set(int x, int y);
		Vector2& set(Vector2& set);

		float getX();
		float getY();

		bool equals(float x, float y);

		std::string toString();

	private:
		float* p_elements = new float[2];
};