#pragma once

#include "Label.h"
#include "ScrollArea.h"

#include <string>
#include <stack>

enum CharacterType {ALPHA, NUMERIC, ALPHANUMERIC, ALL};

/*
 Stores states - used in undoing.
 */
struct FieldState {
	std::string text;
	int cursorIndex;

	FieldState(std::string& text, int cursorIndex) {
		this->text = text;
		this->cursorIndex = cursorIndex;
	}
};

/*
 A simple text field.
 Must use a mono font!
 */
class TextField : public ScrollArea {
	public:
		TextField(const std::string& id, Vector2& localTranslation, float width);
		~TextField();

		void setText(std::string& text);

		void setFont(std::string& font);
		void setFontSize(int fontSize);

		void clear();

		/*
		 Sets the colour of the text and the vertical bar
		 */
		void setForegroundColour(ColourRGBA& colour);

		/*
		 Sets the colour of the box itself
		 */
		void setBackgroundColour(ColourRGBA& colour);

		/*
		 Returns the text currently entered into this text field.
		 */
		std::string& getText();

		/*
		 Sets the maximum number of characters that this field will accept.
		 */
		void setCharacterLimit(int charLimit);

		/*
		 Sets the type of characters this field will accept - ALPHABETICAL, NUMERIC, ALPHANUMERIC, or ALL.
		 */
		void setInputType(CharacterType inputType);

		/*
		 If set to false, this element will be greyed out and not allow input.
		 */
		void setEnabled(bool isEnabled);

		void render();
		void renderPostDescendants();

		void onFocus(Vector2& mousePosition);
		void onLostFocus(Vector2& mousePosition);
		void charPressed(char keyCharacter);
		void charReleased(char keyCharacter);
		void keyPressed(int keyCode);
		void keyReleased(int keyCode);

		void mousePressed(MouseButton mousebutton, Vector2& mousePosition);

	private:
		Label* p_textLabel;					// Pointer to the label that displays the text
		Vector2 selection;					// Stores the current selection, or 0,0 if there isn't anything selected.

		std::string text;					// The text currently in the text field
		std::stack<FieldState> undoStack;	// Every action pushes the current state onto the stack.

		unsigned int charLimit,				// Character limit
					 fieldIndices,			// The number of letters we can render in the field
					 cursorIndex,			// The index the cursor is currently at.
					 localIndex;			// The local index of the cursor

		CharacterType charType = ALL;

		bool isEnabled = true;				// If false, greyed out and doesn't accept text
		bool hasFocus;						// True when the user is inputting text
		bool insertEnabled;					// True if we're replacing text

		DWORD lastVerticalBarToggle;		// The ms at the last time we toggled the vertical bar ('|')
		bool showVerticalBar;				// True if we're drawing the bar at the moment

		float width;						// The width of the text field

		/*
		 Used to convert standard keys ('0', '1', '/', etc.) into their shifted counterparts ('!', '"', '?', etc.)
		 */
		char checkShiftCharacter(char keyCharacter);

		/*
		 Repositions the text label.
		 */
		void updateTextLabel();
};