#include <glut.h>

#include "Paint.h"

#define TICK_RATE 15

/** Define all the glut menus */
void buildMenus();

/** glut callback for drawing the scene */
void drawScene();

/** glut callback for keypress handling */
void handleKeypress(unsigned char key, int x, int y);

/** glut callback for menu handling */
void handleMenu(int val);

/** glut callback for handling mouse clicks */
void handleMouseClick(int buttonNum, int state, int x, int y);

/** glut callback for handling mouse dragging */
void handleMouseMotion(int x, int y);

/** glut callback for handing window resize events */
void handleResize(int w, int h);

/** glut callback for handling an animation timer */
void handleTimer(int val);

// global instance of the paint application
static Paint* paint;

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	paint = new Paint();

	/* glut callback registration */
    glutDisplayFunc(drawScene);
	glutTimerFunc(TICK_RATE, handleTimer, 0);
    glutReshapeFunc(handleResize);
    glutMouseFunc(handleMouseClick);
    glutMotionFunc(handleMouseMotion);
    glutKeyboardFunc(handleKeypress);

	buildMenus();

	paint->loop();

    return 0;
}

void buildMenus() {
	int rectId = glutCreateMenu(handleMenu);
	glutAddMenuEntry("Filled",   RECT_FILL);
	glutAddMenuEntry("Outlined", RECT_NOFILL);

	int colorId = glutCreateMenu(handleMenu);
	glutAddMenuEntry("Red",    RED);
	glutAddMenuEntry("Orange", ORANGE);
	glutAddMenuEntry("Yellow", YELLOW);
	glutAddMenuEntry("Green",  GREEN);
	glutAddMenuEntry("Blue",   BLUE);
	glutAddMenuEntry("Indigo", INDIGO);
	glutAddMenuEntry("Violet", VIOLET);
	glutAddMenuEntry("White",  WHITE);
	glutAddMenuEntry("Black",  BLACK);

	int sizeId = glutCreateMenu(handleMenu);
	glutAddMenuEntry("2",  TWO_S);
	glutAddMenuEntry("4",  FOUR_S);
	glutAddMenuEntry("8",  EIGHT_S);
	glutAddMenuEntry("16", SIXTEEN_S);

	int widthId = glutCreateMenu(handleMenu);
	glutAddMenuEntry("2",  TWO_W);
	glutAddMenuEntry("4",  FOUR_W);
	glutAddMenuEntry("8",  EIGHT_W);
	glutAddMenuEntry("16", SIXTEEN_W);

	glutCreateMenu(handleMenu);
	glutAddMenuEntry("Point",    POINT);
	glutAddMenuEntry("Scribble", SCRIBBLE);
	glutAddMenuEntry("Line",     LINE);

	glutAddSubMenu("Rectangle",  rectId);
	glutAddSubMenu("Color",      colorId);
	glutAddSubMenu("Point Size", sizeId);
	glutAddSubMenu("Line Width", widthId);

	glutAddMenuEntry("Toggle Fade",  FADE);
	glutAddMenuEntry("Clear", CLEAR);
	glutAddMenuEntry("Quit",  QUIT);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void drawScene() {
	paint->drawScene();
}

void handleKeypress(unsigned char key, int x, int y) {
	paint->handleKeypress(key, x, y);
}

void handleMenu(int choice) {
	paint->handleMenu(choice);
}

void handleMouseClick(int buttonNum, int state, int x, int y) {
	paint->handleMouseClick(buttonNum, state, x, y);
}

void handleMouseMotion(int x, int y) {
	paint->handleMouseMotion(x, y);
}

void handleResize(int w, int h) {
	paint->handleResize(w, h);
}

void handleTimer(int val) {
	paint->handleTimer(val);
	glutTimerFunc(TICK_RATE, handleTimer, 0);
}
