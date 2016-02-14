#pragma once

#include "GUIElement.h"
#include "ColourRGBA.h"
#include "GLFont.h"

#include <string>

/*
 A label. By default, labels are 12-pt and shadowed.
 */
class Label : public GUIElement {
	public:
		Label(const std::string& id, const std::string& text, Vector2& position);
		Label(const std::string& id, const std::string& text, int fontSize, Vector2& position);
		Label(const std::string& id, const std::string& text, int fontSize, ColourRGBA& colour, Vector2& position);
		~Label();

		void render();
		
		/*
		 If true, a shadow is rendered.
		 */
		void setShadowed(bool shadow);

		void setFont(const std::string& filePath);
		void setFontSize(int size);

		void setText(const std::string& text);

		std::string getText();
		int getFontSize();

	private:
		GLFont* p_GLFont;
		char* p_text;
		std::string text;

		float size = 12;
		bool dropShadow = true;
};