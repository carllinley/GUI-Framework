#include "Renderer.h"
#include "GUI.h"

#include <iostream>

#include <gl\GL.h>
#include <Windows.h>

void Renderer::pushTransform(GUIElement& element) {
	float halfX = element.p_quad->getHalfX(),
		  halfY = element.p_quad->getHalfY();

	glPushMatrix();
	glTranslatef(element.getLocalTranslation().getX(), element.getLocalTranslation().getY(), 0);
	glScalef(element.getScale(), element.getScale(), 0);

	glPushMatrix();
	glTranslatef(halfX, halfY, 0); // Rotate around centre, not origin
	glRotatef(element.getLocalRotation(), 0, 0.0f, 1.0f);
	glTranslatef(-halfX, -halfY, 0);
}

void Renderer::popTransform(GUIElement& element) {
	glPopMatrix(); // Pop rotation
	glPopMatrix(); // Pop translation and scale
}

void Renderer::render(float timePerFrame, GUIElement& element, RenderType renderType) {
	this->timePerFrame = timePerFrame;
	this->renderType = renderType;

	render(element);
}

void Renderer::render(GUIElement& element) {
	if (renderType == STANDARD) {
		element.update(timePerFrame);
	}

	if (!element.isVisible()) {
		return;
	}

	pushTransform(element);

	// Render this element
	if (renderType == STANDARD) {
		element.render();
	}
	else if (element.isResponsive()) {
		renderPicking(element);
	}

	// Recursively render all descendants
	for (int i = 0; i != element.children.size(); i++) {
		render(*element.children[i]);
	}

	if (renderType == STANDARD) {
		element.renderPostDescendants();
	}

	// Cleanup

	if (element.scrollable) {
		glDisable(GL_SCISSOR_TEST);
	}
	popTransform(element);
}

void Renderer::renderPicking(GUIElement& element) {
	if (element.scrollable) { // Need to apply scissoring
		Vector2* worldTranslation = element.getWorldTranslation();
		Vector2 screenPosition = *worldTranslation + (GUI::get().getDimensions() / 2.0f);
		screenPosition.setX(screenPosition.getX() - 10);
		screenPosition.setY(screenPosition.getY() - 20);

		glEnable(GL_SCISSOR_TEST);
		glScissor((GLint) screenPosition.getX(), (GLint) screenPosition.getY(), (GLint) element.getDimensions().getX(), (GLint)element.getDimensions().getY());

		delete worldTranslation;
	}

	float r = (element.pickIndex & 0x000000FF) / 256.0f,
		  g = ((element.pickIndex & 0x0000FF00) >> 8) / 256.0f,
		  b = ((element.pickIndex & 0x00FF0000) >> 16) / 256.0f; // Gives us a floating point RGB colour

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(r, g, b);

	for (int i = 0; i != 4; i++) {
		glVertex2f(element.p_quad->getVertex(i).getX(), element.p_quad->getVertex(i).getY());
	}
	glEnd();
}