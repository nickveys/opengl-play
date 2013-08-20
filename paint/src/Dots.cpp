#include <stdio.h>
#include <stdlib.h>

#include <glut.h>

#include "Dots.h"

Dots::~Dots() {
	std::vector< struct point2f* >::iterator itr = dots.begin();
	while(itr != dots.end()) {
		struct point2f* p = *itr;
		itr = dots.erase(itr);
		free(p);
	}
}

void Dots::addPoint(GLfloat x, GLfloat y) {
	struct point2f* p = (struct point2f*) malloc(sizeof(struct point2f));
	p->x = x; 
	p->y = y;
	dots.push_back(p);
}

void Dots::paint() {
	glColor4f(getColor().r, getColor().g, getColor().b, getColor().a);
	glPointSize(getPointSize());

	std::vector< struct point2f* >::const_iterator itr = dots.begin();
	glBegin(GL_POINTS);
		while(itr != dots.end()) {
			glVertex2f((*itr)->x, (*itr)->y);
			itr++;
		}
	glEnd();
}
