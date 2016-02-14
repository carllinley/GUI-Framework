#include "GUI.h"
#include "GLFont.h"
#include "MouseListener.h"
#include "Animation.h"

#include <string>
#include <sstream>
#include <iostream>

GUI* GUI::s_instance = NULL;

GUI::GUI(int width, int height) {
	p_size = new Vector2((float) width, (float) height);
	p_mouseCoords = new Vector2();
}

void GUI::updateTooltip(const std::string& tooltip) {
	p_tooltip->setText(tooltip);
	p_tooltip->setVisible(tooltip != "");
}

GUI::~GUI() {
	delete p_state;
	delete p_size;
	delete p_mouseCoords;
	delete p_customCursor;

	// delete p_tooltip;
}

GUI* GUI::createContext(int width, int height) {
	if (s_instance != nullptr) {
		delete s_instance;
	}

	s_instance = new GUI(width, height);
	s_instance->formatTooltip("fonts/franklin_gothic.glf", 10, ColourRGBA::WHITE);
	return s_instance;
}

void GUI::destroyContext() {
	delete s_instance;
}

void GUI::cleanup(GUIElement* element) {
	if (p_currentPicked != nullptr
		&& (p_currentPicked == element || element->ancestorOf(p_currentPicked))) {
		p_currentPicked = nullptr;
	}
	if (p_lastPicked != nullptr
		&& (p_lastPicked == element || element->ancestorOf(p_lastPicked))) {
		p_lastPicked = nullptr;
	}
	if (p_currentFocus != nullptr
		&& (p_currentFocus == element || element->ancestorOf(p_currentFocus))) {
		p_currentFocus = nullptr;
	}
}

void GUI::resize(int newWidth, int newHeight) {
	// yOffset = (newHeight - p_size->getY());
	// std::cout << "Resize Y offset: " << yOffset << std::endl;
}

Vector2& GUI::getDimensions() {
	return *p_size;
}

void GUI::registerElement(GUIElement* p_element) {
	if (freeIndices.size() == 0) {
		p_element->setIndex(indexTrack++);
	}
	else {
		p_element->setIndex(freeIndices.front());
		freeIndices.pop();
	}
	pickingMap[p_element->pickIndex] = p_element;
}

void GUI::unregisterElement(GUIElement* p_element) {
	if (p_element->pickIndex != 0) {
		pickingMap[p_element->pickIndex] = nullptr;
		freeIndices.push(p_element->pickIndex); // Re-use the picking index
	}
}

GLFont* GUI::getFont(const std::string& filePath) {
	if (fontManager.fonts[filePath] == nullptr) {
		GLFont* p_GLFont = new GLFont();
		p_GLFont->Create(filePath.c_str());
		fontManager.fonts[filePath] = p_GLFont;
		std::cout << "Loaded font '" << filePath << "'." << std::endl;

		if (fontManager.fonts[filePath] == nullptr) { // Couldn't load it
			std::cout << "Failed to load font: " << filePath << std::endl;
			return nullptr;
		}
	}
	return fontManager.fonts[filePath];
}

Texture* GUI::getTexture(const std::string& filePath) {
	if (imageManager.textures[filePath] == nullptr) {
		if (imageManager.load(filePath) == nullptr) { // Couldn't load it
			std::cout << "Failed to load image: " << filePath << std::endl;
			return nullptr;
		}
	}
	return imageManager.textures[filePath];
}

void GUI::setCursor(const std::string& fileName) {
	if (fileName != "") {
		if (p_customCursor != nullptr) {
			cursorSet = false;
			delete p_customCursor;
		}
		ShowCursor(false);
		cursorAnimated = false;
		p_customCursor = new GUIImage("customCursor", fileName);
		cursorSet = true;
	}
	else {
		cursorSet = false;
		ShowCursor(true);
	}
}

void GUI::setAnimatedCursor(const std::string & fileName, int frames, int frameDelay, bool unordered) {
	if (p_customCursor != nullptr) {
		cursorSet = false;
		delete p_customCursor;
	}
	ShowCursor(false);

	Animation* animatedCursor = new Animation("customCursor", fileName, 32, frames);
	animatedCursor->setUnordered(unordered);
	cursorAnimated = true;
	p_customCursor = animatedCursor;
	cursorSet = true;
}

void GUI::setFocus(GUIElement& newFocus) {
	if (p_currentFocus != &newFocus) {
		if (p_currentFocus != nullptr) {
			p_currentFocus->onLostFocus(*p_mouseCoords);
		}
		p_currentFocus = &newFocus;
		p_currentFocus->onFocus(*p_mouseCoords);
	}
}

void GUI::formatTooltip(const std::string& font, int size, const ColourRGBA& colour) {
	if (p_tooltip != nullptr) {
		p_tooltip->setFont(font);
		p_tooltip->setFontSize(size);
		p_tooltip->setColour(colour);
	}
	else {
		p_tooltip = new Label("", "", size, Vector2());
		p_tooltip->setColour(colour);
		p_tooltip->setFont(font);
	}
}

BOOL GUI::processMouseAction(MouseButton mouseButton, MouseAction mouseAction, int mouseX, int mouseY) {
	if (mouseAction == MOVED) {
		p_mouseCoords->set(mouseX, mouseY);
	}
	checkingPick = true;

	if (p_currentPicked != p_lastPicked) { // Hover handling
		updateTooltip("");

		if (p_lastPicked != nullptr) {
			p_lastPicked->onHoverOff(*p_mouseCoords);
		}

		if (p_currentPicked != nullptr) {
			p_currentPicked->onHover(*p_mouseCoords);

			if (p_currentPicked->getToolTip() != nullptr) {
				updateTooltip(*p_currentPicked->getToolTip());
			}
		}
		else {
			p_tooltip->setVisible(false);
		}
	}
	p_lastPicked = p_currentPicked;

	if (p_currentFocus != nullptr && mouseAction == PRESSED) { // Focus handling
		p_currentFocus->onLostFocus(*p_mouseCoords);
		p_currentFocus = nullptr;
	}

	if (p_currentPicked == nullptr) { // No need to continue
		return true;
	}

	switch (mouseAction) {
		case MOVED:
			p_currentPicked->mouseMoved(*p_mouseCoords);
		break;

		case PRESSED:
			if (!p_currentPicked->isVisible()) {
				return true;
			}
			p_currentPicked->bringToFront();
			p_currentFocus = (p_currentPicked->getPassFocusElement() == nullptr ? p_currentPicked : p_currentPicked->getPassFocusElement());
			p_currentFocus->onFocus(*p_mouseCoords);
			p_currentPicked->mousePressed(mouseButton, *p_mouseCoords);
		break;

		case RELEASED:
			if (!p_currentPicked->isVisible()) {
				return true;
			}
			p_currentPicked->mouseReleased(mouseButton, *p_mouseCoords);
		break;
	}
	return true;
}

BOOL GUI::processMouseWheel(MouseWheel dir, int mouseX, int mouseY) {
	if (p_currentFocus == nullptr) {
		return true;
	}

	switch (dir) {
		case WHEEL_UP:
			p_currentFocus->mouseWheelUp(*p_mouseCoords);
		break;
	
		case WHEEL_DOWN:
			p_currentFocus->mouseWheelDown(*p_mouseCoords);
		break;
	}
	return true;
}

BOOL GUI::processCharacter(KeyState state, char character) {
	if (p_currentFocus == nullptr) {
		return true;
	}

	switch(state) {
		case UP:
			p_currentFocus->charReleased(character);
		break;
	
		case DOWN:
			p_currentFocus->charPressed(character);
		break;
	}
	return true;
}

BOOL GUI::processKey(KeyState state, int keyID) {
	switch (keyID) {
		case 20: // Caps lock
			if (state == PRESSED && !updatedCapsLock) {
				capsLocked = !capsLocked;
				updatedCapsLock = true;
			}
			else if (state == RELEASED) {
				updatedCapsLock = false;
			}
		break;

		case 16: // Shift
			isShifting = (state == PRESSED);
		break;

		case 17: // CTRL
			isControlling = (state == PRESSED);
		break;
	}

	if (p_currentFocus != nullptr) {
		if (state == DOWN) {
			p_currentFocus->keyPressed(keyID);
		}
		else {
			p_currentFocus->keyReleased(keyID);
		}
	}
	return TRUE;
}

void GUI::update() {
	if (checkingPick) {
		// Render picking colours. Uses per-pixel picking - based on example code found at
		// http://content.gpwiki.org/index.php/OpenGL_Selection_Using_Unique_Color_IDs

		for (int i = 0; i != p_state->children.size(); i++) {
			renderer.render(timePerFrame, *p_state->children[i], PICKING);
		}

		GLubyte rgb[3];

		// Reads the pixel under the mouse from the framebuffer and stores it in the GLubyte array 'rgb'
		glReadPixels((GLint) p_mouseCoords->getX(), (GLint) ((p_size->getY() - p_mouseCoords->getY() - 40)), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, rgb);

		int uniqueID = rgb[0] + rgb[1] + rgb[2];

		if (uniqueID != 0 && uniqueID < indexTrack) {
			p_currentPicked = pickingMap[uniqueID];
		}
		else {
			p_currentPicked = nullptr;
		}
		checkingPick = false;
	}
}

void GUI::render() {
	long startTime = GetTickCount();

	for (int i = 0; i != p_state->children.size(); i++) {
		renderer.render(timePerFrame, *p_state->children[i], STANDARD);
	}

	if (cursorSet || p_tooltip->isVisible()) { // No point translating if we're not actually drawing
		Vector2 worldSpace = *p_mouseCoords - (*p_size / 2);

		glPushMatrix();
		glTranslatef(worldSpace.getX() + 5, -worldSpace.getY() - 50, 0);

		if (cursorSet) {
			if (cursorAnimated) {
				Animation* p_animCursor = static_cast<Animation*>(p_customCursor);
				p_animCursor->update(timePerFrame);
				p_animCursor->render();
			}
			else {
				p_customCursor->render();
			}
		}
		if (p_tooltip->isVisible()) {
			p_tooltip->render();
		}
		glPopMatrix();
	}

	glFlush();

	long duration = (GetTickCount() - startTime);

	/*renderTimes.push_back(duration);

	if (GetTickCount() - start > 1000) {
		int total = 0;

		for (int i = 0; i != renderTimes.size(); i++) {
			total += renderTimes[i];
		}
		float average = (float) total / (float) renderTimes.size();
		averagingRenderTimes.push_back(average);
		std::cout << "Rendered " << renderTimes.size() << " times, in an average of " << average << "ms." << std::endl;
		renderTimes.erase(renderTimes.begin(), renderTimes.end());
		std::cout << renderTimes.size() << std::endl;
		start = GetTickCount();

		if (averagingRenderTimes.size() % 10 == 0) {
			float avTotal = 0;

			for (int i = 0; i != averagingRenderTimes.size(); i++) {
				avTotal += averagingRenderTimes[i];
			}
			float absAverage = (float) avTotal / (float) averagingRenderTimes.size();
			std::cout << "Absolute average: " << absAverage << std::endl;
			averagingRenderTimes.erase(averagingRenderTimes.begin(), averagingRenderTimes.end());
		}
	}*/

	if (duration != 0) {
		timePerFrame = 1.0f - (1.0f / duration);
	}
	else {
		timePerFrame = 1.0f;
	}
}

bool GUI::typingInCapitals() {
	return capsLocked;
}

bool GUI::shiftHeld() {
	return isShifting;
}

bool GUI::ctrlHeld() {
	return isControlling;
}

void GUI::setState(GUIState* guiState) {
	p_state = guiState;
}

float GUI::getTimePerFrame() {
	return timePerFrame;
}

GUIState* GUI::getState() {
	return p_state;
}

GUIElement* GUI::getFocus() {
	return p_currentFocus;
}
