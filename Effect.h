#pragma once

#include "Vector2.h"
#include "ColourRGBA.h"

/*
 The superclass for all effects.
 */
class Effect {
	friend class GUIElement;

	public:
		Effect(float speed);
		~Effect();

		/*
		 Starts the effect.
		 Any initial modifications to the element should happen here.
		 */
		virtual void start();

		/*
		 Updates the effect.
		 timePerFrame - the current time per frame, used to keep speed consistent.
		 */
		virtual void update(float timePerFrame) = 0;

		/*
		 Called when this effect is finished or interrupted.
		 */
		virtual void onCompletion();

		/*
		 Convenience - almost all effects have a speed.
		 */
		virtual void setSpeed(float speed);

		void setElement(GUIElement* element);
		bool hasInitialised();

	protected:
		GUIElement* p_element;
		bool isComplete,
			 initialised;	// True if the element has been set

		float speed;
};

/*
The element is shrunk into nothing.
It's set invisible and returned to its original scale when the effect finishes.
*/
class ShrinkOut : public Effect {
	public:
		ShrinkOut(float shrinkSpeed);

		void start();
		void update(float timePerFrame);
		void onCompletion();

	private:
		float originScale;

		float* p_scale; // Maintain a pointer to the current scale, bit sketchy but avoids fannying about
};

/*
 The element is grown in from nothing.
 It's set visible and to scale when the effect finishes.
 */
class GrowIn : public Effect {
	public:
		GrowIn(float growSpeed);

		void start();
		void update(float timePerFrame);
		void onCompletion();

	private:
		float originScale;

		float* p_scale; // Maintain a pointer to the current scale, bit sketchy but avoids fannying about
};

enum RotationDirection {CLOCKWISE, ANTICLOCKWISE};

/*
Rotates the element around its centre.
*/
class Rotate : public Effect {
	friend class GUIElement;

	public:
		Rotate(float rotationSpeed);
		Rotate(float rotationSpeed, RotationDirection rotationDirection);
		Rotate(float rotationSpeed, RotationDirection rotationDirection, bool returnToOrigin);
		
		void start();
		void update(float timePerFrame);
		void onCompletion();
		void setSpeed(float speed);

	private:
		float currentAngle, // The angle we've rotated by so far.
			  originAngle; // The element's rotation when the effect started.

		RotationDirection direction = CLOCKWISE; // The direction we're rotating in - CLOCKWISE by default.

		bool returnOrigin = true; // If true, the element will be set back to its original rotation when the effect finishes. True by default.
};

/*
The element gradually fades in.
*/
class FadeIn : public Effect {
	public:
		FadeIn(float fadeSpeed);

		void start();
		void update(float timePerFrame);
		void onCompletion();

	private:
		float currentAlpha; // The current alpha of this element and all of its descendants.
};

/*
The element gradually fades out.
*/
class FadeOut : public Effect {
	public:
		FadeOut(float fadeSpeed);

		void start();
		void update(float timePerFrame);
		void onCompletion();

	private:
		float currentAlpha; // The current alpha of this element and all of its descendants.
};

/*
 The element pulses - that is, fades in and out rapidly.
 */
class Pulse : public Effect {
	public:

		/*
		 Creates a pulse effect using the specified speed.
		 */
		Pulse(float pulseSpeed);

		/*
		 Creates a pulse effect using the specified speed and intensity.
		 Intensity defines how transparent the element gets as a floating point from 0 - 1, where 0 fades it out completely.
		 */
		Pulse(float pulseSpeed, float pulseIntensity);

		void start();
		void update(float timePerFrame);
		void onCompletion();

	private:
		float currentAlpha,		// The current alpha of this element and all of its descendants.
			  intensity;		// How transparent the element gets - percentage of opaque.

		bool direction; // True if we're fading out, false if we're fading in
};

/*
 The element disappears and reappears.
 */
class Flash : public Effect {

	public:
		Flash(float flashSpeed);
	
		/*
		 noOfTimes - the amount of times the element flashes before the effect stops.
		 */
		Flash(float flashSpeed, int numberOfTimes);

		void start();
		void update(float timePerFrame);
		void onCompletion();
		void setSpeed(float speed);

	private:
		float progress;
		int times, flashCount;
};

/*
 The element is set to the specified colour.
 It's returned to its original colour when the effect is stopped.
 */
class ChangeColour : public Effect {
	public:
		ChangeColour(const ColourRGBA& colour);
		ChangeColour(ColourRGBA& colour);

		void start();
		void update(float timePerFrame);
		void onCompletion();

	private:
		ColourRGBA originalColour,
				   newColour;
};