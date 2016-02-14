#pragma once

#include "Renderer.h"
#include "Vector2.h"
#include "Label.h"
#include "TGAReader.h"
#include "GUIState.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

class GUIElement;
class GUIImage;

struct Texture {
	GLuint glID;
	int width, height;
};

/*
 Stores textures to avoid re-loading images, re-creating textures, and wasting memory.
 */
struct TextureManager {
	std::unordered_map<std::string, Texture*> textures;

	~TextureManager() {
		for (auto kv : textures) {
			delete kv.second;
		}
	}

	Texture* load(const std::string& filePath) {
		Texture* texture = new Texture();

		unsigned char* buffer = nullptr;
		int bitsPerPixel = 0;
		int channelsPerPixel = 0;

		EImageReaderStatus result = ImageReader::LoadTGA(filePath.c_str(), &buffer, &texture->width, &texture->height, &bitsPerPixel, &channelsPerPixel);

		if (result == E_IMAGEREADER_SUCCESS) {
			glGenTextures(1, &texture->glID);
			glBindTexture(GL_TEXTURE_2D, texture->glID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

			textures[filePath] = texture;

			std::cout << "Loaded texture '" << filePath << "'\n" << texture->width << "x" << texture->height << ", BPP: " <<
			bitsPerPixel << ", CPP: " << channelsPerPixel << ", Texture ID: " << texture->glID << std::endl;
			return texture;
		}
		else {
			std::cout << "Failed to load texture '" << filePath << "'." << std::endl;
			delete texture;
			return nullptr;
		}
	}
};

/*
 Stores instances of GLFont to avoid wasting memory.
 */
struct FontManager {
	std::unordered_map<std::string, GLFont*> fonts;

	~FontManager() {
		for (auto kv : fonts) {
			delete kv.second;
		}
	}
};

/*
 Manages the GUI system.
 */
class GUI {
	public:
		// std::vector<int> renderTimes;
		// std::vector<float> averagingRenderTimes;
		// long start = 0;

		~GUI();

		static GUI* createContext(int width, int height);
		static void destroyContext();

		inline static GUI& get() {
			return *s_instance;
		}

		/*
		 If the element is the current picked, focused, etc. it is set to nullptr.
		 */
		void cleanup(GUIElement* element);

		/*
		 Should be called whenever the window is resized. If not done, picking will be offset.
		 */
		void resize(int newWidth, int newHeight);

		/*
		 Returns the GUI's dimensions.
		 */
		Vector2& getDimensions();

		/*
		 Adds the specified element to the pickable list.
		 */
		void registerElement(GUIElement* p_element);

		/*
		 Removes the specified element from the pickable list.
		 */
		void unregisterElement(GUIElement* p_element);

		/*
		 Returns the specified GLFont, assuming it has been registered. If not, returns nullptr.
		 */
		GLFont* getFont(const std::string& fontName);

		/*
		 Returns the specified texture.
		 If it hasn't been loaded before, it's loaded and returned. If it can't be loaded, a nullptr is returned.
		 */
		Texture* getTexture(const std::string& imagePath);

		/*
		 Sets the cursor to the specified image. Cursors should be 32x32, pointing at the top left.
		 */
		void setCursor(const std::string& fileName);
		
		/*
		 Sets the cursor to the specified animation. Each cell must be 32x32, pointing at the top left.
		 */
		void setAnimatedCursor(const std::string& fileName, int frames, int frameDelay, bool unordered);

		/*
		 Overrides focusing, giving the specified element focus.
		 */
		void setFocus(GUIElement& newFocus);

		/*
		 Configure the label to be used for tooltips.
		 */
		void formatTooltip(const std::string& font, int size, const ColourRGBA& colour);

		/*
		 Called by the Win32 window. Handles mouse button presses/releases.
		 */
		BOOL processMouseAction(MouseButton buttonID, MouseAction actionID, int mouseX, int mouseY);

		BOOL processMouseWheel(MouseWheel dir, int mouseX, int mouseY);

		/*
		 Called by the Win32 window. Handles characters and such.
		 */
		BOOL processCharacter(KeyState state, char charPressed);

		/*
		 Called by the Win32 window. Handles shift, caps lock, and the like.
		 */
		BOOL processKey(KeyState state, int keyID);

		/*
		 Updates the GUI - call this before clearing and rendering the application.
		 */
		void update();

		/*
		 Renders the GUI - call this after rendering the application.
		 */
		void render();

		/*
		 Returns true if holding shift / caps lock is enabled, and we should type capital letters.
		 */
		bool typingInCapitals();

		/*
		 Returns true if shift is held p_down.
		 */
		bool shiftHeld();

		/*
		 Returns true if CTRL is held p_down.
		 */
		bool ctrlHeld();

		/*
		 Sets the current state of the GUI.
		 */
		void setState(GUIState* guiState);

		/*
		 Returns the current time per frame.
		 */
		float getTimePerFrame();

		GUIState* getState();
		GUIElement* getFocus();

	private:
		GUI(int width, int height);
		static GUI* s_instance; // Singleton instance

		FontManager fontManager = FontManager();
		TextureManager imageManager = TextureManager();
		Renderer renderer = Renderer();

		GUIState* p_state; // The current 'state' of the GUI.

		GUIElement* p_currentPicked; // A pointer to the GUIElement currently under the mouse.
		GUIElement* p_lastPicked; // A pointer to the GUIElement under the mouse on the last frame. Used to detect gaining/losing focus.
		GUIElement* p_currentFocus; // A pointer to the GUIElement last pressed by the mouse. Not to be confused with mouse focusing!

		Label* p_tooltip;				// The label to be used by tooltips

		/*
		 Custom cusors
		 */
		GUIImage* p_customCursor;		// The set custom cursor
		bool cursorSet,					// True if a custom cursor is set. != nullptr crashes.
			 cursorAnimated;			// True if the cursor is an Animation, not just a GUIImage

		/*
		 Variables for per-pixel picking
		 */
		int indexTrack = 1;				// Starts at one - zero represents null
		bool checkingPick;				// If true, the next frame is rendered in picking mode before being rendered properly
		Vector2* p_mouseCoords;			// Pointer to a vector storing the current mouse coordinates

		float timePerFrame = 1.0f;		// The current time per frame
		long lastCheck;					// The tick count when TPF was last updated

		bool capsLocked,				// If true, caps lock is enabled.
			 updatedCapsLock,			// Used internally to detect the state of caps lock.
			 isShifting,				// True if a shift key is held p_down
			 isControlling;				// True if a CTRL key is held p_down

		Vector2* p_size;				// Stores the screen's dimensions.

		// float yOffset;				// Stores the resized Y's difference from the origin Y, used to keep picking accurate.

		/*
		 A map storing every element with a unique index. This index is converted to a colour and used in picking.
		 */
		std::unordered_map<int, GUIElement*> pickingMap;
		std::queue<int> freeIndices; // A queue of freed indices to re-use.

		void updateTooltip(const std::string& tooltip);
};