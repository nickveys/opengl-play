#include <stdio.h>

#include <glut.h>

#include "Paintable.h"
#include "Tool.h"

/* default tool setup */
struct color4f Tool::color = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat Tool::size = 2.0f;
GLfloat Tool::width = 2.0f;
bool Tool::fill = false;

Tool::Tool() {
	mouseCurrentlyDown = false;
}

void Tool::mouseDown(struct point2f p) {
	mouseCurrentlyDown = true;
}

void Tool::mouseMove(struct point2f p) {
	/* do nothing */
}

void Tool::mouseUp(struct point2f p) {
	/* ignore up clicks from clicking the menus */
	if(mouseCurrentlyDown) {
		mouseCurrentlyDown = false;
	}
}

void Tool::setColor(GLfloat r, GLfloat g, GLfloat b) {
	color.r = r;
	color.g = g;
	color.b = b;
}

void Tool::notifyIntermediatePaintableCreated(Paintable* p) {
	std::vector< ToolListener* >::const_iterator itr = toolListeners.begin();
	while(itr != toolListeners.end()) {
		(*itr)->intermediatePaintableCreated(p);
		itr++;
	}
}

void Tool::notifyFinalPaintableCreated(Paintable* p) {
	std::vector< ToolListener* >::const_iterator itr = toolListeners.begin();
	while(itr != toolListeners.end()) {
		(*itr)->finalPaintableCreated(p);
		itr++;
	}
}
