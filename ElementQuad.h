#pragma once

#include "Vector2.h"

class ElementQuad {
	friend class GUIElement;

	public:
		ElementQuad(Vector2& position, Vector2& size);
		~ElementQuad();

		void resize(Vector2& size);
		void translate(Vector2& translation);
		void scale(float scale);
		void rotate_Degrees(float angleRads);

		Vector2& getVertex(int i);

		/*
		 Returns this element's local translation.
		 */
		Vector2& getTranslation();

		/*
		 Returns this quad's scaling factor.
		 */
		float getScale();

		/*
		 Returns this quad's rotation, in degrees.
		 */
		float getRotaton();

		float getHalfX();
		float getHalfY();

		/*
		 Returns a pointer to this quad's vertices. Used by effects.
		 */
		Vector2* getVertices();

	private:
		// const static float DEG_TO_RADS;

		Vector2* p_vertexList;
		Vector2* p_translation;
		Vector2* p_halfDimensions; // For rotation

		float _rotation;
		float _scale;

		void init(Vector2* p_vertexList);

		/*
		 Returns a pointer to the current translation. Used internally - hence GUIElement's friendship.
		 */
		Vector2* translation();
};