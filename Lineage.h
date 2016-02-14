#pragma once

#include <string>
#include <vector>

/*
 Manages lineage - ancestors elements and descendant elements.
 Introduced 05/12 to eliminate redundancy between GUIElement and GUIState.
 */
class Lineage {
	friend class Renderer;
	friend class GUIElement;
	friend class GUIState;

	public:
		~Lineage();

		/*
		 Adds the specified element as a child. Returns a pointer to the element for convenience.
		 */
		virtual GUIElement* attachChild(GUIElement* child);

		/*
		 Removes and deletes the specified child along with all of its descendants.
		 */
		void detachChild(GUIElement* child);
		
		/*
		 Removes and deletes the specified child along with all of its descendants.
		 */
		void detachChild(const std::string& id);

		/*
		 Removes the specified child and its descendants, but *does not* anything from memory.
		 As such, you must handle the memory yourself!
		 This would be used if, for example, a child needs to switch parents.
		 */
		virtual void detachChild_Retain(GUIElement* child);

		/*
		 Detaches and deletes all children.
		 */
		void detachAllChildren();

		/*
		 Returns true if the specified element is a child of this element.
		 */
		bool hasChild(GUIElement* checkChild);
		bool hasChild(const std::string& checkChildID);

		/*
		 Recursively parses all of this element's descendants for the specified element.
		 Better use hasChild if possible, it's less time complex. This could end p_up parsing a lot of elements! O(n log n)?
		 */
		GUIElement* getDescendant(const std::string& checkDescendantID);

		/*
		 Returns the child with the specified ID, or nullptr if it does not exist.
		 */
		GUIElement* getChild(const std::string& id);

		/*
		 Returns a reference to the vector storing this element's children.
		 */
		std::vector<GUIElement*>& getChildren();

		/*
		 Returns this element's parent element, or nullptr if it is attached to the state.
		 */
		GUIElement* getParentElement();

		/*
		 Returns this element's parent - could be an instance of GUIState or GUIElement.
		 If you want an explicit GUIElement (most cases), use getParentElement().
		 */
		Lineage* getParent();

	protected:
		bool isState; // True: GUIState / False: GUIElement

		GUIElement* p_parent;
		std::vector<GUIElement*> children;

		GUIElement* check(GUIElement* element, const std::string& descendant);
};