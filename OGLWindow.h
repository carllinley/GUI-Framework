#pragma once

#include "GUI.h"

#include <Windows.h>

class OGLWindow {
	private:
		HWND m_hwnd;	// Handle to a window
		HDC m_hdc;		// Handle to a device context
		HGLRC m_hglrc;	// Handle to a GL rendering context

		GUI* p_gui;		// GUI system

		int m_width, m_height;

	protected:
		HGLRC createOpenGLContext (HDC hdc);
		BOOL destroyOGLContext();
		BOOL initWindow(HINSTANCE hInstance, int width, int height);
		void initOGLState();

	public:
		static const wchar_t* WINDOW_TITLE;

		OGLWindow(HINSTANCE hInstance, int width, int height);
		~OGLWindow();

		BOOL processMouseWheel(MouseWheel wheel, int x, int y);
		BOOL processMouse(MouseButton button, MouseAction action, int x, int y);
		BOOL processKey(KeyState state, char character, UINT msg);

		void render();
		void resize(int width, int height);
		void setVisible(BOOL visible);
		void destroyOGLWindow();
};