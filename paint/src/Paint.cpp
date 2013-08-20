#include <iostream>

#include <stdio.h>

#include <glut.h>

#include "Dots.h"
#include "DotTool.h"
#include "LineTool.h"
#include "RectangleTool.h"
#include "Paint.h"

Paint::Paint() {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);
    glutInitWindowSize(WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT);
    glutInitWindowPosition(-1, -1);
    glutCreateWindow(WINDOW_TITLE_BASE);

	canvas = new Canvas();
	currentTool = new DotTool(true);
	currentTool->addToolListener(this);

    glViewport(0, 0, WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT);
}

Paint::~Paint() {
	delete canvas;
}

void Paint::drawScene() {
	glClear(GL_COLOR_BUFFER_BIT);
    canvas->drawScene();
	glutSwapBuffers();
}

void Paint::handleKeypress(unsigned char key, int x, int y) {
	switch(key) {
		case 0x1B: exit(0);
		default:
			printf("[paint] Unknown keypress 0x%X '%c' @ (%d, %d)\n", key, key, x, y);
	}
}

void Paint::handleMenu(int choice) {
	switch(choice) {
		case POINT:       switchTool(new DotTool(true));        break;
		case SCRIBBLE:    switchTool(new DotTool(false));       break;
		case LINE:        switchTool(new LineTool());           break;
		case RECT_FILL:   switchTool(new RectangleTool(true));  break;
		case RECT_NOFILL: switchTool(new RectangleTool(false)); break;

		case WHITE:  Tool::setColor(           1.0f,            1.0f,            1.0f); break;
		case BLACK:  Tool::setColor(           0.0f,            0.0f,            0.0f); break;
		case RED:    Tool::setColor(           1.0f,            0.0f,            0.0f); break;
		case ORANGE: Tool::setColor(           1.0f, 160.0f / 255.0f,            0.0f); break;
		case YELLOW: Tool::setColor(           1.0f,            1.0f,            0.0f); break;
		case GREEN:  Tool::setColor(           0.0f,            1.0f,            0.0f); break;
		case BLUE:   Tool::setColor(           0.0f,            0.0f,            1.0f); break;
		case INDIGO: Tool::setColor( 75.0f / 255.0f,            0.0f,  30.0f / 255.0f); break;
		case VIOLET: Tool::setColor(139.0f / 255.0f,            0.0f,            1.0f); break;

		case TWO_S:     Tool::setPointSize(2.0f);  break;
		case FOUR_S:    Tool::setPointSize(4.0f);  break;
		case EIGHT_S:   Tool::setPointSize(8.0f);  break;
		case SIXTEEN_S: Tool::setPointSize(16.0f); break;

		case TWO_W:     Tool::setLineWidth(2.0f);  break;
		case FOUR_W:    Tool::setLineWidth(4.0f);  break;
		case EIGHT_W:   Tool::setLineWidth(8.0f);  break;
		case SIXTEEN_W: Tool::setLineWidth(16.0f); break;

		case FADE: canvas->enableFade(!canvas->isFading()); break;

		case CLEAR: canvas->clear(); break;

		case QUIT: exit(0);

		default:
			printf("[paint] {{WARN}} Unknown menu choice %d selected\n", choice);
	}
}

void Paint::switchTool(Tool* newTool) {
	if(currentTool != NULL) {
		delete currentTool;
	}
	currentTool = newTool;
	currentTool->addToolListener(this);
}

void Paint::handleMouseClick(int buttonNum, int state, int x, int y) {
	// we only care about the first button
	if(buttonNum != 0) return;

	// pass on the click to the current tool
	switch(state) {
		case GLUT_DOWN:
			if(currentTool != NULL) {
				currentTool->mouseDown(pointFromViewXY(x, y));
			}
			break;
		case GLUT_UP:
			if(currentTool != NULL && currentTool->isMouseCurrentlyDown()) {
				currentTool->mouseUp(pointFromViewXY(x, y));
			}
			break;
	}
}

void Paint::handleMouseMotion(int x, int y) {
	if(currentTool != NULL) {
		currentTool->mouseMove(pointFromViewXY(x, y));
	}
}

void Paint::handleResize(int w, int h) {
    glViewport(0, 0, w, h);
}

void Paint::handleTimer(int val) {
	canvas->tick();
}

struct point2f Paint::pointFromViewXY(int x, int y) {
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	struct point2f p = {
		WORLD_X1 * x / (float) w,
		WORLD_Y1 * (h - y) / (float) h
	};
    return p;
}

void Paint::loop() {
	glutMainLoop();
}

void Paint::intermediatePaintableCreated(Paintable* p) {
	canvas->setActivePaintable(p);
	glutPostRedisplay();
}

void Paint::finalPaintableCreated(Paintable* p) {
	canvas->setActivePaintable(NULL);
	canvas->addPaintable(p);
	glutPostRedisplay();
}
