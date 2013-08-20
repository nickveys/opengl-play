#include "structs.h"

#include "Rectangle.h"

Rectangle::Rectangle(bool filled) : Paintable() {
	start.x = start.y = end.x = end.y = -1.0f;
	this->filled = filled;
}

void Rectangle::paint() {
	glColor4f(getColor().r, getColor().g, getColor().b, getColor().a);
	glLineWidth(getLineWidth());

	if(filled) {
		glBegin(GL_POLYGON);
	} else {
		glBegin(GL_LINE_LOOP);
	}
		glVertex2f(start.x, start.y);
		glVertex2f(start.x, end.y);
		glVertex2f(end.x, end.y);
		glVertex2f(end.x, start.y);
	glEnd();

	/*
	 * attempt to bevel the edges of the unfilled rectangle.  this seems to get weird with a
	 * line width of 8.0f, the math doesn't work out quite right and the edges get clipped. 
	 */
	if(!filled) {
		glLineWidth(1.0f);
		float delta = getLineWidth() / 2.0f;
		float xMin = min(start.x, end.x);
		float xMax = max(start.x, end.x);
		float yMin = min(start.y, end.y);
		float yMax = max(start.y, end.y);
		glBegin(GL_POLYGON);
			glVertex2f(xMin, yMin);
			glVertex2f(xMin - delta, yMin);
			glVertex2f(xMin, yMin - delta);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex2f(xMin, yMax);
			glVertex2f(xMin - delta, yMax);
			glVertex2f(xMin, yMax + delta);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex2f(xMax, yMin);
			glVertex2f(xMax + delta, yMin);
			glVertex2f(xMax, yMin - delta);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex2f(xMax, yMax);
			glVertex2f(xMax + delta, yMax);
			glVertex2f(xMax, yMax + delta);
		glEnd();
	}
}
