#pragma once

#include "GUIElement.h"

enum RenderType {STANDARD, PICKING};

/*
 A single place to handle parent/child transformations and rendering.
 Implemented 05/12, the original system was very redundant.
 */
class Renderer {
	public:
		void render(float timePerFrame, GUIElement& element, RenderType renderType);

	private:
		float timePerFrame; // No point passing them recursively
		RenderType renderType;

		void render(GUIElement& element);

		void renderPicking(GUIElement& element);

		void pushTransform(GUIElement& element);
		void popTransform(GUIElement& element);
};