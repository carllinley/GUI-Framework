#pragma once
#pragma once

#include "GUIElement.h"

/*
 A simple image element.
 NOTE: Currently only supports TGA files.
 */
class GUIImage : public GUIElement {

	public:
		/*
		 Creates a new Image element using the specified image. The element's dimensions match those of the image.
		 */
		GUIImage(const std::string id, std::string fileName);

		/*
		 Creates a new Image element using the specified image and dimensions.
		 The image won't be cut, but may be scaled weirdly if the dimensions are too large.
		 */
		GUIImage(const std::string id, std::string fileName, Vector2& dimensions);

		/*
		 Applies image tiling.
		 */
		void setTiling(int tilesX, int tilesY);

		void render();

		/*
		 Returns the width of this image's texture.
		 */
		float getTexWidth();

		/*
		 Returns the height of this image's texture.
		 */
		float getTexHeight();

	protected:
		GLuint textureID;
		Vector2 tiling = Vector2(1, 1);

	private:
		std::string fileName;
};