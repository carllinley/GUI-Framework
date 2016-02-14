#pragma once

#include "GUIElement.h"
#include "TextField.h"
#include "Label.h"
#include "ScrollPane.h"
#include "GUIImage.h"

#include <queue>

/*
 Basic chatbox - demos front-end custom elements.
 */
class Chatbox : public GUIImage {
	public:
		Chatbox(const std::string& id, Vector2& position, float width);

		void pushMessage(const std::string& user, const std::string& string);

	private:
		TextField* p_inputField;
		ScrollPane* p_messageArea;

		std::vector<Label*> entryLabels; // Cheap hack, I don't have time to do this properly

		unsigned int bufferSize = 32;
};