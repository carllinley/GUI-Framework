#include "ComboBox.h"
#include "Effect.h"
#include "GUI.h"

#include <iostream>

bool intervene = true;

void press(GUIElement& parent, Button& button) {
	if (button.getID().length() > 7 && button.getID().substr(0, 7) == "option_") {
		ComboBox& comboBox = static_cast<ComboBox&> (*parent.getParentElement()); // The parent is the container, not the combobox

		std::string selectedOption = button.getID().substr(7);
		comboBox.setSelected(selectedOption);
		intervene = true;
	}
	else {
		ComboBox& comboBox = static_cast<ComboBox&> (parent); // The parent is the combobox

		if (intervene) { // Open
			GUI::get().setFocus(comboBox);
			comboBox.toggleDropdown();
		}
		intervene = !intervene;
	}
}

ComboBox::ComboBox(const std::string& id, Vector2& localTranslation, float width) : GUIElement(id, localTranslation) {
	this->width = width;

	p_buttonContainer = new GUIElement(id + "_container", Vector2(), Vector2(width, 0)); // An empty element to store the buttons
	p_buttonContainer->setVisible(false);

	GUIImage* downIcon = new GUIImage(id + "_down_arrow", "images/scroll/DOWN.tga");
	downIcon->setResponsive(false);

	dropButton = new Button(id + "_drop", Vector2(width, -1), "images/scroll/SCROLL.tga");
	dropButton->assignImages("images/scroll/SCROLL.tga", "images/scroll/SCROLL_HOVER.tga", "images/scroll/SCROLL_PRESSED.tga");
	dropButton->attachChild(downIcon);
	dropButton->setOnPress(press);

	fieldButton = createButton("", 0);
	attachChild(fieldButton);

	attachChild(p_buttonContainer);
	attachChild(dropButton);
}

ComboBox::~ComboBox() {
}

void ComboBox::toggleDropdown() {
	droppedDown = !droppedDown;
	p_buttonContainer->setVisible(droppedDown);
}

void ComboBox::renderPostDescendants() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0f);

	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(width + 1, 0);
	glVertex2f(width + 1, 18);
	glVertex2f(0, 18);
	glEnd();

	if (droppedDown) {
		glBegin(GL_POLYGON);
		glColor3f(0, 0, 0);
		glVertex2f(0, 0);
		glVertex2f(width + 1, 0);
		glVertex2f(width + 1, -18 * (float) optionCount);
		glVertex2f(0, -18 * (float) optionCount);
		glEnd();
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ComboBox::setBackground(GUIImage* image) {
}

void ComboBox::setFont(const std::string& filePath) {
	std::vector<GUIElement*> buttons = p_buttonContainer->getChildren();
	
	for (int i = 0; i != buttons.size(); i++) {
		Button* button = static_cast<Button*>(buttons[i]);
		button->getLabel()->setFont(filePath);
	}
}

void ComboBox::setSelected(const std::string& option) {
	selected = option;
	fieldButton->getLabel()->setText(option);

	if (GUI::get().getState() != nullptr) {
		GUI::get().getState()->valueUpdate(*this, option);
	}
}

void ComboBox::setOptions(int count, const char* ...) {
	optionCount = count - 1;

	p_buttonContainer->setLocalTranslation(Vector2(0, (float) (optionCount + 1) * -18));

	va_list optionList;

	va_start(optionList, count);
	for (int i = 0; i != count; i++) {
		const char* option = va_arg(optionList, const char*);
		addOption(option);

		if (i == 0) {
			setSelected(std::string(option));
		}
	}
	va_end(optionList);

	optionCount = count;
}

void ComboBox::onFocus(Vector2& mousePosition) {
}

void ComboBox::onLostFocus(Vector2& mousePosition) {
	if (droppedDown) {
		toggleDropdown();
	}
}

Button* ComboBox::createButton(const char* option, int optionCount) {
	Label* label = new Label("button_label_" + std::string(option), (char*) option, 10, ColourRGBA(), Vector2(4, 18));
	label->setShadowed(false);

	Button* optionButton = new Button("option_" + std::string(option), Vector2(0, (float) optionCount * 18), Vector2(width, 18));
	optionButton->setLabel(label);
	optionButton->setColour(ColourRGBA::WHITE);
	optionButton->setOnPress(press);
	return optionButton;
}

void ComboBox::addOption(const char* option) {
	Button* optionButton = createButton(option, optionCount);
			optionButton->addEffect(TRIG_HOVER, new ChangeColour(ColourRGBA(ColourRGBA::BLUE)));
			optionButton->setHoverForegroundColour(ColourRGBA(ColourRGBA::WHITE));

	elements[optionCount--] = new ComboElement(option, optionButton);
	p_buttonContainer->attachChild(optionButton);
}

void ComboBox::removeOption(const char* option) {
}

int ComboBox::getSelectedIndex() {
	return selectedIndex;
}

const char* ComboBox::getSelectedValue() {
	return elements[selectedIndex]->option;
}

bool ComboBox::isDropped() {
	return droppedDown;
}
