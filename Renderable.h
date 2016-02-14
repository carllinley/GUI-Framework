#pragma once

#include <Windows.h>
#include <gl/GL.h>

/*
 Declares a class as renderable.
 */
class Renderable {
	public:
		virtual void render() = 0;
};