#include "ScrollPane.h"
#include "GUI.h"

#include <cmath>

ScrollPane::ScrollPane(const std::string& id, Vector2& localTranslation, Vector2& dimensions) : GUIElement(id, localTranslation, dimensions) {
	p_scrollArea = new ScrollArea(id + "_scrollArea", Vector2(), dimensions - Vector2(20, 40));
	p_scrollElementContainer = new GUIElement(id + "_container", Vector2()); // Parent of all attached elements
	p_scrollBar = new ScrollBar(id + "_scrollBar", Vector2(dimensions.getX() - 20, 0), dimensions.getY() - 60);

	setResponsive(false);

	p_scrollArea->setPassFocusElement(p_scrollBar);
	setPassFocusElement(p_scrollBar);

	p_scrollArea->attachChild(p_scrollElementContainer);
	Lineage::attachChild(p_scrollArea);
	Lineage::attachChild(p_scrollBar);
}

bool ScrollPane::up(float amount) {
	if (scrollPos < internalHigh / 4) {
		p_scrollElementContainer->translate(0, -amount);
		scrollPos++;
		updateThumb();
		return true;
	}
	return false;
}

bool ScrollPane::down(float amount) {
	if (scrollPos > 0) {
		p_scrollElementContainer->translate(0, amount);
		scrollPos--;
		updateThumb();
		return true;
	}
	return false;
}

void ScrollPane::floor() {
	p_scrollElementContainer->getLocalTranslation().setY(internalLow + 5);
}

void ScrollPane::setBackground(const std::string& filePath) {
	p_scrollArea->setBackground(filePath);
}

GUIElement* ScrollPane::attachChild(GUIElement* element) {
	if (element->getLocalTranslation().getY() > internalHigh) {
		internalHigh = element->getLocalTranslation().getY();
	}
	if (element->getLocalTranslation().getY() < internalLow) {
		internalLow = element->getLocalTranslation().getY();
	}
	updateThumb();
	return p_scrollElementContainer->attachChild(element);
}

GUIElement* ScrollPane::attachImmediateChild(GUIElement* element) {
	return Lineage::attachChild(element);
}

void ScrollPane::mouseWheelUp(Vector2& mousePosition) {
	up(4.0f * GUI::get().getTimePerFrame());
}

void ScrollPane::mouseWheelDown(Vector2& mousePosition) {
	down(4.0f * GUI::get().getTimePerFrame());
}

ScrollBar* ScrollPane::getScrollBar() {
	return p_scrollBar;
}

void ScrollPane::updateThumb() {
	p_scrollBar->recalculateThumb(internalHigh, internalLow, scrollPos, p_scrollArea->getDimensions().getY());
}

ScrollBar::ScrollBar(const std::string& id, Vector2& localTranslation, float size) : GUIElement(id, localTranslation) {
	this->size = size;

	Vector2 buttonDimensions = Vector2(20, 20);

	// Create p_up / p_down buttons

	GUIImage* upIcon = new GUIImage(id + "_up_icon", "images/scroll/UP.tga");
	upIcon->setResponsive(false);

	p_up = new Button(id + "_up", Vector2(0, size), buttonDimensions);
	p_up->assignImages("images/scroll/SCROLL.tga", "images/scroll/SCROLL_HOVER.tga", "images/scroll/SCROLL_PRESSED.tga");
	p_up->attachChild(upIcon);

	GUIImage* downIcon = new GUIImage(id + "_down_icon", "images/scroll/DOWN.tga");
	downIcon->setResponsive(false);

	p_down = new Button(id + "_down", Vector2(), buttonDimensions);
	p_down->assignImages("images/scroll/SCROLL.tga", "images/scroll/SCROLL_HOVER.tga", "images/scroll/SCROLL_PRESSED.tga");
	p_down->attachChild(downIcon);

	// Create p_track and p_thumb

	p_track = new Button(id + "_track", Vector2(0, 20), "images/scroll/TRACK.tga");
	p_track->resize(20, size - 20);

	p_thumb = new Button(id + "_thumb", Vector2(0, 40));
	p_thumb->assignImages("images/scroll/THUMB.tga", "images/scroll/THUMB_HOVER.tga", "images/scroll/THUMB_PRESSED.tga");
	p_thumb->resize(20, size - 20);

	p_thumb->setPassFocusElement(this);
	p_track->setPassFocusElement(this);
	p_up->setPassFocusElement(this);
	p_down->setPassFocusElement(this);

	p_track->attachChild(p_thumb);
	attachChild(p_track);
	attachChild(p_up);
	attachChild(p_down);
}

void ScrollBar::scrollDown(float amount) {
	if (p_parent == nullptr) {
		return; // Inert / overridden
	}
	if (static_cast<ScrollPane&>(*p_parent).down(amount)) {
		// p_thumb->translate(0, -jump);
	}
}

void ScrollBar::scrollUp(float amount) {
	if (p_parent == nullptr) {
		return; // Inert / overridden
	}
	if (static_cast<ScrollPane&>(*p_parent).up(amount)) {
		// p_thumb->translate(0, jump);
	}
}

void ScrollBar::mouseWheelUp(Vector2& mousePosition) {
	scrollDown(4.0f * GUI::get().getTimePerFrame());
}

void ScrollBar::mouseWheelDown(Vector2& mousePosition) {
	scrollUp(4.0f * GUI::get().getTimePerFrame());
}

void ScrollBar::recalculateThumb(float high, float low, float pos, float viewportHeight) {
	// Based on example code found at http://stackoverflow.com/questions/16366795/how-to-calculate-the-size-of-scroll-bar-thumb

	float contentHeight = abs(high) + abs(low),
		  viewableRatio = viewportHeight / contentHeight,
		  scrollBarArea = (viewportHeight - 40) / 4,
		  thumbHeight = scrollBarArea * viewableRatio;
	
	float scrollTrackSpace = contentHeight - viewportHeight,
		  scrollThumbSpace = viewportHeight - thumbHeight;
	
	jump = scrollTrackSpace / scrollThumbSpace;

	p_thumb->resize(20, thumbHeight);
	p_thumb->setLocalTranslation(Vector2(0, jump * pos));
}

void ScrollBar::update(float timePerFrame) {
	if (p_up->isPressed() && (GetTickCount() - lastUpdate > 50)) {
		scrollUp(timePerFrame * 4.0f);
		lastUpdate = GetTickCount();
	}
	if (p_down->isPressed() && (GetTickCount() - lastUpdate > 50)) {
		scrollDown(timePerFrame * 4.0f);
		lastUpdate = GetTickCount();
	}
}
