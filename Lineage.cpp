#include "Lineage.h"
#include "GUI.h"
#include "GUIElement.h"

Lineage::~Lineage() {
	detachAllChildren();
}

GUIElement* Lineage::attachChild(GUIElement* child) {
	if (child->p_parent != nullptr) {
		child->p_parent->detachChild_Retain(child); // MY child now
	}

	if (!isState) {
		child->p_parent = static_cast<GUIElement*>(this); // Must be a GUIElement
	}
	
	if (child->getEffect(TRIG_ATTACHED) != nullptr) {
		child->playEffect(child->getEffect(TRIG_ATTACHED));
	}

	children.push_back(child);

	if (child->isPickable()) {
		GUI::get().registerElement(child);
	}
	return child;
}

void Lineage::detachChild(GUIElement* child) {
	if (child == nullptr) {
		return;
	}

	if (child->getEffect(TRIG_DETACHED) != nullptr) { // Will be detached when the effect is complete
		child->playEffect(child->getEffect(TRIG_DETACHED));
	}
	else {
		GUI::get().unregisterElement(child);
		children.erase(std::find(children.begin(), children.end(), child));
		delete child;
	}
}

void Lineage::detachChild(const std::string& id) { // Convenience method
	detachChild(getChild(id));
}

void Lineage::detachChild_Retain(GUIElement* child) {
	if (child == nullptr) {
		return;
	}
	children.erase(std::find(children.begin(), children.end(), child));
}

void Lineage::detachAllChildren() {
	while (children.size() != 0) {
		detachChild(children[0]);
	}
}

std::vector<GUIElement*>& Lineage::getChildren() {
	return children;
}

GUIElement* Lineage::getParentElement() {
	return p_parent;
}

Lineage* Lineage::getParent() {
	if (p_parent == nullptr) {
		return GUI::get().getState();
	}
	return p_parent;
}

GUIElement* Lineage::check(GUIElement* element, const std::string& descendant) {
	for (int i = 0; i != element->children.size(); i++) {
		if (*element->children[i] == descendant) {
			return element->children[i];
		}

		GUIElement* e = check(element->children[i], descendant);

		if (e != nullptr) {
			return e;
		}
	}
	return nullptr;
}

bool Lineage::hasChild(GUIElement* checkChild) {
	for (int i = 0; i != children.size(); i++) {
		if (children[i] == checkChild) {
			return true;
		}
	}
	return false;
}

bool Lineage::hasChild(const std::string& checkChildID) {
	return getChild(checkChildID) != nullptr;
}

GUIElement* Lineage::getDescendant(const std::string& checkDescendantID) {
	for (int i = 0; i != children.size(); i++) {
		if (*children[i] == checkDescendantID) {
			return children[i];
		}
		GUIElement* checkLineage = check(children[i], checkDescendantID);
		
		if (checkLineage != nullptr) {
			return checkLineage;
		}
	}
	return nullptr;
}

GUIElement* Lineage::getChild(const std::string& id) {
	for (int i = 0; i != children.size(); i++) {
		if (*children[i] == id) {
			return children[i];
		}
	}
	return nullptr;
}