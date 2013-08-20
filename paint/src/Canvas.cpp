#include <glut.h>

#include "Canvas.h"
#include <Paintable.h>

Canvas::Canvas() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	// set up alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

    gluOrtho2D(WORLD_X0, WORLD_X1, WORLD_Y0, WORLD_Y1);

	// default no paintable and no fading
	activePaintable = NULL;
	fading = false;
}

Canvas::~Canvas() {
	clearPaintables();
}

void Canvas::clear() {
	clearPaintables();
	glutPostRedisplay();
}

void Canvas::clearPaintables() {
	std::vector< Paintable* >::iterator itr = paintables.begin();
	while(itr != paintables.end()) {
		Paintable* p = *itr;
		itr = paintables.erase(itr);
		delete p;
	}
	activePaintable = NULL;
}

void Canvas::addPaintable(Paintable* p) {
	if(p != NULL) {
		paintables.push_back(p);
	}
}

void Canvas::setActivePaintable(Paintable* p) {
	activePaintable = p;
}

void Canvas::drawScene() {
	std::vector< Paintable* >::iterator itr = paintables.begin();
	while(itr != paintables.end()) {
		Paintable* p = *itr;
		if(p->isDead()) {
			itr = paintables.erase(itr);
			delete p;
		} else {
			p->paint();
			itr++;
		}
	}

	if(activePaintable != NULL) {
		activePaintable->paint();
	}
}

void Canvas::tick() {
	if(fading) {
		/*
		 * if we're fading, drop every alpha by a small amount, and if they're 0, kill them off
		 */
		std::vector< Paintable* >::const_iterator itr = paintables.begin();
		while(itr != paintables.end()) {
			struct color4f c = (*itr)->getColor();
			if(c.a <= 0.0f) {
				(*itr)->kill();
			} else {
				c.a -= 0.005f;
				(*itr)->setColor(c);
			}
			itr++;
		}
		glutPostRedisplay();
	}
}
