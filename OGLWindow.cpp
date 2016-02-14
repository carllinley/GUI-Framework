#include "OGLWindow.h"
#include "Resource.h"
#include "DemoState.h"

#include <cctype>
#include <iostream>
#include <gl/GL.h>

const wchar_t* OGLWindow::WINDOW_TITLE = L"OpenGL GUI Framework";

OGLWindow::OGLWindow(HINSTANCE hInstance, int width, int height) {
	initWindow(hInstance, width, height);

	p_gui = GUI::createContext(width, height);
	p_gui->setState(new DemoState("DemoState", p_gui));
}

OGLWindow::~OGLWindow() {
	destroyOGLContext();
	GUI::destroyContext();
}

HGLRC OGLWindow::createOpenGLContext(HDC hdc) {
	unsigned int pixelformat;
	HGLRC hglrc;

	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Support Window
		PFD_SUPPORT_OPENGL |						// Format Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request RGBA Format
		32,											// Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits mask
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// Z-Buffer depth
		0,											// Stencil Buffer
		0,											// Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(pixelformat = ChoosePixelFormat(hdc, &pfd))
	 || !SetPixelFormat(hdc, pixelformat, &pfd)
	 || !(hglrc = wglCreateContext(hdc))
	 || !wglMakeCurrent(hdc, hglrc)) {
		return 0;
	}
	return hglrc;
}

void OGLWindow::destroyOGLWindow() {
	destroyOGLContext();
	DestroyWindow(m_hwnd);

	m_hwnd = NULL;
	m_hdc = NULL;
}

BOOL OGLWindow::destroyOGLContext() {
	if (m_hglrc) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hglrc);
		m_hglrc = NULL;
	}
	return ReleaseDC(m_hwnd, m_hdc);
}

BOOL OGLWindow::initWindow(HINSTANCE hInstance, int width, int height) {
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
							OGLWindow::WINDOW_TITLE, OGLWindow::WINDOW_TITLE, WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
							0, 0, width, height, NULL, NULL, hInstance, NULL);

	if (!m_hwnd) {
		return FALSE;
	}

	m_hdc = GetDC(m_hwnd);

	if (!(m_hglrc = createOpenGLContext(m_hdc))) {
		return FALSE;
	}

	m_width = width;
	m_height = height;

	return TRUE;
}

void OGLWindow::setVisible(BOOL visible) {
	ShowWindow(m_hwnd, visible? SW_SHOW : SW_HIDE);
}

void OGLWindow::render() {
	p_gui->update();

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	p_gui->render();

	SwapBuffers(m_hdc);
}

void OGLWindow::resize(int width, int height) {
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.5 * width, 0.5 * width, -0.5 * height, 0.5 * height, -1.0, 1.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	p_gui->resize(width, height);
	return;
}

void OGLWindow::initOGLState() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

BOOL OGLWindow::processMouseWheel(MouseWheel wheel, int x, int y) {
	return p_gui->processMouseWheel(wheel, x, y);
}

BOOL OGLWindow::processMouse(MouseButton button, MouseAction action, int x, int y) {
	return p_gui->processMouseAction(button, action, x, y);
}

BOOL OGLWindow::processKey(KeyState state, char character, UINT msg) {
	if (msg == WM_CHAR) {
		return p_gui->processCharacter(state, character);
	}
	if (character < 0 || !std::isalnum(character)) {
		return p_gui->processKey(state, character);
	}
	return true;
}
