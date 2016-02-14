#pragma once

#include "Vector.h"

/**
 * Provides a level of abstraction on top of Vector allowing three-dimensional vectors.
 **/
class Vector3 : public Vector {
	public:
		Vector3();
		Vector3(float x, float y, float z);
		Vector3(Vector& conversion);

		void setX(float x);
		void setY(float y);
		void setZ(float z);
		void set(float x, float y, float z);

		float getX();
		float getY();
		float getZ();
};