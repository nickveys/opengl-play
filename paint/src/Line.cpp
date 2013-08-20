#include <stdio.h>

#include "Line.h"

Line::Line() : Paintable() {
	start.x = start.y = end.x = end.y = -1.0f;
}

void Line::paint() {
	glColor4f(getColor().r, getColor().g, getColor().b, getColor().a);
	glLineWidth(getLineWidth());

	glBegin(GL_LINES);
		glVertex2f(start.x, start.y);
		glVertex2f(end.x, end.y);
	glEnd();
}
