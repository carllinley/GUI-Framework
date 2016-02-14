#include "Chatbox.h"

Chatbox::Chatbox(const std::string& id, Vector2& position, float width) : GUIImage(id, "demo/images/chatbox/CHAT_BACK.tga") {
	setLocalTranslation(position);
	resize(width, 200);
	setDraggable(true);

	p_messageArea = new ScrollPane(id + "_scroll", Vector2(10, 38), Vector2(width - 20, 190));
	p_messageArea->setBackground("demo/images/chatbox/CHAT_FORE.tga");

	p_inputField = new TextField(id + "_input", Vector2(10, 10), width - 20);
	p_inputField->setCharacterLimit(32);
	p_inputField->setBackground("demo/images/chatbox/CHAT_FIELD.tga");
	p_inputField->setForegroundColour(ColourRGBA(ColourRGBA::WHITE));

	setPassFocusElement(p_messageArea);

	for (int i = 0; i != bufferSize; i++) {
		entryLabels.push_back(new Label("", "", 9, Vector2(4, (i + 1) * 10))); // We don't care about their IDs
		p_messageArea->attachChild(entryLabels[i]);
	}

	p_messageArea->floor();

	attachChild(p_messageArea);
	attachChild(p_inputField);
}

void Chatbox::pushMessage(const std::string& user, const std::string& text) {
	if (text == "") {
		return;
	}

	for (int i = bufferSize - 1; i != 0; i--) {
		entryLabels[i]->setText(entryLabels[i - 1]->getText());
	}
	entryLabels[0]->setText(user + ": " + text);
}
