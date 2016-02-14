#pragma once

/*
 Stores colour percentages.
 */
class ColourRGBA {
	public:
		const static ColourRGBA BLACK, WHITE, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW;

		/*
		 Creates a new colour instance using the specified values.
		 */
		ColourRGBA(float red, float green, float blue, float alpha);

		/*
		 Creates a new colour instance, defaults to black.
		 */
		ColourRGBA();

		/*
		 Creates a copy of the specified colour.
		 */
		ColourRGBA(const ColourRGBA& toCopy);

		~ColourRGBA();

		ColourRGBA& operator=(ColourRGBA& toAssign);
		ColourRGBA& operator-(ColourRGBA& toSubtract);
		ColourRGBA& operator*(ColourRGBA& toMult);
		ColourRGBA& operator*(float scalar);

		ColourRGBA& operator/(float scalar);

		void set(const ColourRGBA& colour);

		void setRed(float red);
		void setGreen(float green);
		void setBlue(float blue);
		void setAlpha(float alpha);

		float getRed();
		float getGreen();
		float getBlue();
		float getAlpha();

		void normalise();

	private:
		float* p_values;
};