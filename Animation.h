#pragma once

#include "GUIImage.h"

/*
 An animated image.
 */
class Animation : public GUIImage {
	public:
		/*
		 A simple single looping animation.
		 cellSize - the size, in pixels, of each frame.
		 int frameCount - the number of frames.
		 */
		Animation(const std::string& id, const std::string& imageFile, int cellSize, int frameCount);

		/*
		 A series of animations - each row is a separate animation.
		 cellSize - the size, in pixels, of each frame.
		 int* frameCount - an array storing the number of frames for each animation.
		 */
		Animation(const std::string& id, const std::string& imageFile, int cellSize, int* frameCounts);

		~Animation();

		/*
		 Overridden from GUIImage - we need to render the image with altered texture coordinates.
		 */
		void render();

		/*
		 Sets the current animation.
		 */
		void setAnimation(int animation);

		/*
		 Sets the delay between each frame, in milliseconds. 500ms by default.
		 */
		void setDelay(int delay);

		/*
		 If true, the animation loops. If false, it simply remains on the last frame once complete.
		 */
		void setLooping(bool loop);

		/*
		 If true, the animation plays in a random order.
		 */
		void setUnordered(bool unordered);

	private:
		bool loops = true,
			 unordered;

		int* p_frameCounts;

		int lastChangeTick = 0,
			currentFrame = 0,
			currentAnimation = 0;

		unsigned int delayMS = 500;

		Vector2 tilePercentage;
};