#pragma once

#include "Lineage.h"
#include "MouseListener.h"
#include "KeyListener.h"

#include "ColourRGBA.h"
#include "ElementQuad.h"
#include "Renderable.h"
#include "Vector2.h"

#include <iostream>
#include <vector>
#include <unordered_map>

class Effect;

enum EffectTrigger {
	GENERIC,
	TRIG_ATTACHED,
	TRIG_DETACHED,
	TRIG_HOVER,
	TRIG_HOVER_OFF,
	TRIG_PRESSED // Unused
};

enum GradiantType {TO_BOTTOM, TO_TOP, TO_LEFT, TO_RIGHT, NO_GRADIANT};

/*
 Stores dragging data.
 */
struct Draggable {
	Vector2 lastUpdate;
	bool isDragging;
};

/*
 Stores gradiant data.
 */
struct Gradiant {
	GradiantType type;
	ColourRGBA colour;
};

/**
 The superclass of all GUI elements.
 Elements must have an ID and a position.
 **/
class GUIElement : public Renderable, public MouseListener, public KeyListener, public Lineage {
	friend class GUI;
	friend class GUIState;
	friend class Renderer;
	friend class Lineage;

	public:
		GUIElement(const std::string& id, Vector2& position);
		GUIElement(const std::string& id, Vector2& position, Vector2& size);
		~GUIElement();

		/*
		 Returns true if this element has the specified ID.
		 */
		bool operator==(const char* id);
		bool operator==(const std::string& id);
		bool operator!=(const std::string& id);

		/*
		 Brings this element to the front of the parent's list of children.
		 */
		void bringToFront();

		/*
		 Returns a pointer to the effect set to play on the specified trigger event.
		 */
		Effect* getEffect(EffectTrigger trigger);

		/*
		 Called right before rendering.
		 */
		virtual void update(float timePerFrame);
		
		/*
		 Called before rendering any descendants. It's not abstract as some elements do not actually NEED to render - see Slider.
		 */
		virtual void render();

		/*
		 Called after rendering all descendants. Used if you want something
		 "always on top", e.g borders.
		 */
		virtual void renderPostDescendants();

		/*
		 The specified effect will be performed during the specified event.
		 */
		void addEffect(EffectTrigger trigger, Effect* effect);

		/*
		 Performs the specified effect immediately.
		 */
		void playEffect(Effect* effect);

		/*
		 Returns to the generic effect, or nothing if there isn't one.
		 */
		void resetEffects();

		/*
		 Resizes this element to match the specified dimensions.
		 */
		virtual void resize(float width, float height);

		/*
		 If an element is responsive, it can be picked and responds to mouse actions as appropriate.
		 */
		void setResponsive(bool isResponsive);

		/*
		 If an element is resizable, it can be scaled by dragging the edges.
		 */
		void setResizable(bool isScalable);

		/*
		 If an element is draggable, it can be moved by dragging it.
		 */
		void setDraggable(bool isDraggable);

		/*
		 If the element is textured, this creates a tinge - if not, it sets its colour. Colour is white by default.
		 */
		void setColour(ColourRGBA& colour);
		void setColour(const ColourRGBA& colour);

		/*
		Sets the element's gradiant - uses the default intensity, 1.25.
		*/
		void setGradiant(GradiantType type);

		/*
		 Sets the element's gradiant and intensity. Positive intensity provides a darker gradiant.
		 */
		void setGradiant(GradiantType type, float intensity);

		/*
		 Sets the element's colour gradiant.
		 */
		void setGradiant(GradiantType type, ColourRGBA& endColour);

		/*
		 If set to false, this element will not be drawn.
		 */
		void setVisible(bool isVisible);

		/*
		 Sets a tooltip to be displayed whilst the user hovers over this element.
		 */
		void setToolTip(const std::string& tooltip);

		/*
		 Overrides this element's alpha value, and recursively does the same for all descendants.
		 Used by the fade in / out effect to minimise computation.
		 */
		void descendAlpha(float* alpha);

		/*
		 Locally translates this element by the specified x and y.
		 */
		void translate(float x, float y);

		/*
		 Sets translation relative to the parent element.
		 */
		void setLocalTranslation(Vector2& translation);

		/*
		 Sets rotation relative to the parent element. Uses degrees.
		 */
		void setLocalRotation(float rotation_degrees);

		/*
		 Scales this element using the given scalar.
		 */
		void setScale(float scale);

		/*
		 If set, the element will pass events onto the specified element.
		 */
		void setPassFocusElement(GUIElement* p_element);

		/*
		 Returns this element's front-end provided ID.
		 */
		const std::string& getID();

		/*
		 Returns this element's position relative to the world. Bit of a hack, but hey.
		 Note: You are responsible for the freeing the memory!
		 */
		Vector2* getWorldTranslation();

		/*
		 Returns this element's position relative to its parent.
		 */
		Vector2& getLocalTranslation();

		/*
		 Returns this element's scale.
		 */
		float getScale();

		/*
		 Returns this element's rotation.
		 */
		float getLocalRotation();

		/*
		 Returns this element's colour.
		 */
		ColourRGBA& getColour();

		/*
		 Returns a copy of this element's quad's dimensions.
		 */
		Vector2 getDimensions();

		/*
		 Returns a pointer to this element's tooltip.
		 */
		std::string* getToolTip();

		/*
		 Returns true if this element is draggable.
		 */
		bool isDraggable();

		/*
		 Returns true if this element is resizable.
		 */
		bool isResizable();

		/*
		 Returns true if this element has a colour gradiant applied.
		 */
		bool hasGradiant();

		// Pointer access

		/*
		 Returns a pointer to the base float storing scale.
		 */
		float* p_scale();

		GUIElement* getPassFocusElement();

		/*
		 Returns this element's alpha colour.
		 */
		float getAlpha();

		/*
		 Returns a pointer to this element's quad.
		 */
		ElementQuad* getQuad();

		/*
		 Returns true if this element is an ancestor of the specified element.
		 */
		bool ancestorOf(GUIElement* checkAncestor);

		bool isVisible();
		bool isResponsive();
		bool isPickable();
		bool isPressed();

	protected:
		std::string elementID;
		ElementQuad* p_quad;
		GUIElement* p_passFocus;

		Draggable* p_dragData;
		Draggable* p_resizeData; // Stores resizing data - it's Draggable with a different purpose.

		Gradiant* p_gradiant;
		Effect* p_currentEffect;
		std::string* p_tooltip;

		float* p_alpha; // Overrides the element's alpha value.

		ColourRGBA* p_colour = new ColourRGBA(ColourRGBA::WHITE);

		bool visible = true,
			 pickable = true,
			 scrollable = false,
			 pressed; // True if this element is currently pressed

		int pickIndex;

		// Mouse events - overridable by subclasses

		/*
		 Called whenever a mouse button is pressed over this element
		 */
		virtual void mousePressed(MouseButton mouseButton, Vector2& mousePosition);

		/*
		 Called whenever a mouse button is released over this element
		 */
		virtual void mouseReleased(MouseButton mouseButton, Vector2& mousePosition);

		/*
		 Called whenever the mouse is moved over this element
		 */
		virtual void mouseMoved(Vector2& mousePosition);
		
		/*
		 Called whenever the mouse wheel is moved whilst over this element
		 */
		virtual void mouseWheelUp(Vector2& mousePosition);
		virtual void mouseWheelDown(Vector2& mousePosition);

		/*
		 Called when the mouse first moves over this element
		 */
		virtual void onHover(Vector2& mousePosition);

		/*
		 Called when the mouse moves off of this element
		 */
		virtual void onHoverOff(Vector2& mousePosition);

		/*
		 Called when the mouse clicks this element
		 */
		virtual void onFocus(Vector2& mousePosition);

		/*
		 Follows onFocus, when the mouse clicks another element (or nothing)
		 */
		virtual void onLostFocus(Vector2& mousePosition);

		// Key events - again overridable by subclasses

		/*
		 Called when the specified character is pressed - includes holding it p_down.
		 */
		virtual void charPressed(char keyCharacter);

		/*
		 Called when the specified character is released. Limited to actual characters.
		 */
		virtual void charReleased(char keyCharacter);

		/*
		 Called when non-character keys are pressed.
		 */
		virtual void keyPressed(int keyID);

		/*
		 Called when non-character keys are released.
		 */
		virtual void keyReleased(int keyID);

	private:
		std::unordered_map<EffectTrigger, Effect*> effects;

		void init(const std::string& id, Vector2& position, Vector2& dimensions);

		inline void setIndex(int index) {
			pickIndex = index;
		}
};