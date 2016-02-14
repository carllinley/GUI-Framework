#include "TextField.h"
#include "GUI.h"

#include <iostream>
#include <cctype>
#include <math.h>

TextField::TextField(const std::string& id, Vector2& localTranslation, float width) : ScrollArea(id, localTranslation, Vector2(width, 24)) {
	this->width = width;

	p_textLabel = new Label(id + "_label", "", Vector2(4, 10));
	p_textLabel->setColour(ColourRGBA::BLACK);
	p_textLabel->setShadowed(false);
	p_textLabel->setFont("fonts/mono/unispace.glf");
	p_textLabel->setFontSize(10);
	attachChild(p_textLabel);

	fieldIndices = (int) (width / (float) p_textLabel->getFontSize());
}

TextField::~TextField() {
	GUIElement::~GUIElement();
}

void TextField::setText(std::string& text) {
	this->text = text;
	this->cursorIndex = this->text.size();
	
}

void TextField::setFont(std::string& font) {
	p_textLabel->setFont(font);
}

void TextField::setFontSize(int fontSize) {
	p_quad->resize(Vector2(width, (float) fontSize * 3));
	fieldIndices = (int) (width / (float) p_textLabel->getFontSize());
}

void TextField::setForegroundColour(ColourRGBA& colour) {
	p_textLabel->setColour(colour);
}

void TextField::setBackgroundColour(ColourRGBA& colour) {
	setColour(colour);
}

std::string& TextField::getText() {
	return text;
}

void TextField::setCharacterLimit(int charLimit) {
	this->charLimit = charLimit;
}

void TextField::setInputType(CharacterType inputType) {
	charType = inputType;
}

void TextField::setEnabled(bool isEnabled) {
	this->isEnabled = isEnabled;
}

void TextField::render() {
	ScrollArea::render();

	if (hasFocus && GetTickCount() - lastVerticalBarToggle >= 500) { // Handles the veritcal bar: |
		showVerticalBar = !showVerticalBar;
		lastVerticalBarToggle = GetTickCount();
	}
	if (showVerticalBar) {
		glLineWidth(2.5f);
		glColor4f(p_textLabel->getColour().getRed(), p_textLabel->getColour().getGreen(), p_textLabel->getColour().getBlue(), p_textLabel->getAlpha());
		glBegin(GL_LINES);

		float x;

		// Surely there's an easy way of doing this
		if (cursorIndex < fieldIndices) {
			x = (float) 10 + (cursorIndex * p_textLabel->getFontSize());
		}
		else {
			x = (float) 10 + (fieldIndices - (text.size() - cursorIndex)) * p_textLabel->getFontSize();
			// x = (float) 10 + (fieldIndices - 2) * p_textLabel->getFontSize();
		}

		glVertex2f(x, 2);
		glVertex2f(x, (float) p_textLabel->getFontSize() * 2);
		glEnd();
	}
}

void TextField::renderPostDescendants() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0f);

	glBegin(GL_POLYGON);
	glColor4f(0, 0, 0, getAlpha());
	glVertex2f(0, 0);
	glVertex2f(width + 1, 0);
	glVertex2f(width + 1, 24);
	glVertex2f(0, 24);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void TextField::clear() {
	setText(std::string(""));
	updateTextLabel();
}

char TextField::checkShiftCharacter(char keyCharacter) {
	// Pretty un-sexy switch block, I condensed it to save clutter. May have been nicer using an associative array.
	// I bet there's a function somewhere that does this, I didn't see anything though (and it only took like two minutes to do).
	switch (keyCharacter) {
		case '1':	return '!';
		case '2':	return '\"';
		case '3':	return '£';
		case '4':	return '$';
		case '5':	return '%';
		case '6':	return '^';
		case '7':	return '&';
		case '8':	return '*';
		case '9':	return '(';
		case '0':	return ')';
		case '-':	return '_';
		case '=':	return '+';
		case '`':	return '¬';
		case ';':	return ':';
		case '\'':	return '@';
		case ',':	return '<';
		case '.':	return '>';
		case '/':	return '?';
		case '\\':	return '|';
		case '[':	return '{';
		case ']':	return '}';
		case '#':	return '~';
	}
	return keyCharacter; // Uppercase by default
}

void TextField::updateTextLabel() {
	p_textLabel->setText(text);

	if (cursorIndex > fieldIndices) {
		signed int offset = cursorIndex - fieldIndices; // Needs to be explicitly signed

		p_textLabel->setLocalTranslation(Vector2((float) 4 + (offset + 2) * -p_textLabel->getFontSize() / 2, 10)); // A larger offset would be good - so the cursor is centred
	}
	else {
		p_textLabel->setLocalTranslation(Vector2(4, 10)); // Its base position
	}
}

void TextField::onFocus(Vector2& mousePosition) {
	hasFocus = true;
}

void TextField::onLostFocus(Vector2& mousePosition) {
	hasFocus = false;
	showVerticalBar = false;
}

void TextField::charPressed(char keyCharacter) {
	if (keyCharacter == 13 && GUI::get().getState() != nullptr) { // Front-end may need an update
		GUI::get().getState()->onReturn(*this, getText());
	}

	if (!isEnabled
	 || keyCharacter == 13 // Return
	 || keyCharacter == 8 // Backspace
	 || (charLimit > 0 && text.size() >= charLimit)) {
		return;
	}

	if (charType != ALL) {
		if ((charType == NUMERIC && !std::isdigit(keyCharacter))
		 || (charType == ALPHA && !std::isalpha(keyCharacter))
		 || (charType == ALPHANUMERIC && !std::isalnum(keyCharacter))) {
			return;
		}
	}
	else {
		std::string preCursor = text.substr(0, cursorIndex),
					postCursor = text.substr(cursorIndex, text.size());

		if (GUI::get().ctrlHeld()) {
			OpenClipboard(NULL);
			std::string clipboardText;

			switch (keyCharacter) {
				case 22: // Paste
					clipboardText = static_cast<char*> (GetClipboardData(CF_TEXT));

					undoStack.push(FieldState(text, cursorIndex));

					if (charLimit != 0 && (text.size() + clipboardText.size() > charLimit)) {
						clipboardText = clipboardText.substr(0, charLimit - (text.length() + clipboardText.length())); // Can't paste it all, it's too long - cut it and paste what we can
					}
					text = preCursor + clipboardText + postCursor;
					cursorIndex += clipboardText.length();
				break;

				// TODO if time permits
				
				case 1: // Select all

				break;

				case 3: // Copy

				break;

				case 24: // Cut
				break;

				case 26: // Undo
					if (undoStack.size() > 0) {
						FieldState undo = undoStack.top();

						text = undo.text;
						cursorIndex = undo.cursorIndex;
						undoStack.pop();
					}
				break;
			}
			CloseClipboard();
			keyCharacter = NULL;
		}
		else if (!GUI::get().shiftHeld()) {
			if (GUI::get().typingInCapitals()) {
				keyCharacter = std::toupper(keyCharacter);
			}
		}
		else {
			keyCharacter = checkShiftCharacter(keyCharacter);
		}

		if (keyCharacter != NULL) {
			undoStack.push(FieldState(text, cursorIndex));

			bool floorCursor = (cursorIndex == text.size());

			if (insertEnabled) {
				if (postCursor.size() > 0) {
					postCursor = postCursor.substr(1, postCursor.size());
				}
			}
			text = preCursor + keyCharacter + postCursor;

			if (floorCursor) {
				cursorIndex = text.size();
			}
			else {
				cursorIndex++;
			}
		}

		if (GUI::get().getState() != nullptr) { // Front-end may need an update
			GUI::get().getState()->valueUpdate(*this, getText());
		}

		p_textLabel->setText(text);
		showVerticalBar = false;
		lastVerticalBarToggle = GetTickCount();
	}
	updateTextLabel();
}

void TextField::charReleased(char keyCharacter) {
}

void TextField::keyPressed(int keyCode) {
	switch (keyCode) {
		case 8: // Backspace
			if (text.size() > 0 && cursorIndex > 0) {
				if (cursorIndex == text.size()) {
					text = text.substr(0, text.size() - 1);
					cursorIndex = text.size();
				}
				else {
					std::string preCursor = text.substr(0, cursorIndex),
								postCursor = text.substr(cursorIndex, text.size());

					text = preCursor.substr(0, preCursor.size() - 1) + postCursor;
					cursorIndex--;
				}
				p_textLabel->setText(text);
				showVerticalBar = true;
				lastVerticalBarToggle = GetTickCount();
			}
		break;
		
		case 33: // Page p_up
		case 35: // End
			cursorIndex = text.size();
		break;

		case 34: // Page p_down
		case 36: // Home
			cursorIndex = 0;
		break;

		case 37: // Left arrow
			if (!GUI::get().shiftHeld()) {
				if (cursorIndex > 0) {
					cursorIndex--;
				}
				showVerticalBar = true;
				lastVerticalBarToggle = GetTickCount();
			}
			else { // Selection
			}
		break;

		case 39: // Right arrow
			if (!GUI::get().shiftHeld()) {
				if (cursorIndex < text.size()) {
					cursorIndex++;
				}
				showVerticalBar = true;
				lastVerticalBarToggle = GetTickCount();
			}
			else { // Selection
			}
		break;

		case 45: // Insert
			insertEnabled = !insertEnabled;
		break;

		case 46: // Delete
			if (cursorIndex < text.size()) {
				cursorIndex++; // This isn't your average everyday lazy... this is...
				keyPressed(8); // ADVANCED LAZY!
			}
		break;
	}
}

void TextField::keyReleased(int keyCode) {
}

void TextField::mousePressed(MouseButton mousebutton, Vector2& mousePosition) {
	int newIndex = cursorIndex;

	Vector2* worldTranslation = getWorldTranslation();
	Vector2 localMouseClick = *worldTranslation + (mousePosition - GUI::get().getDimensions() / 2).setY(0);
	
	// This USED to work but I've fluffed it somewhere, no time to fix.
	localIndex = (int) round(((localMouseClick.getX() * 100 / width) * fieldIndices) / 100) - 40; // Fixed index, as if there was no scrolling

	if (cursorIndex > fieldIndices) {
		int offset = cursorIndex - fieldIndices;
		cursorIndex = localIndex + offset;

		if (cursorIndex > text.size()) {
			cursorIndex = text.size();
		}
		updateTextLabel();
	}
	else {
		if (localIndex > text.size()) {
			cursorIndex = text.size();
		}
		else {
			cursorIndex = localIndex;
		}
	}
	delete worldTranslation;
}