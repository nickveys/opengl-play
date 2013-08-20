#include <stdlib.h>

#include <GL/glut.h>
#include <glui.h>

#include "BezierCurve.h"
#include "BezierSurface.h"
#include "Camera.h"

/* available menu selections */
enum {
	NEW_CURVE, MODIFY, VIEW, CLEAR, QUIT, U, V,
	NEW_SURFACE, SURF_OK, SURF_X, SURF_Y, CAMERA
};

Camera camera;

/* current bezier shapes */
BezierCurve *curve = NULL;
BezierSurface *surface = NULL;

/* u, v parameters for tangets and normals */
float u, v;

/* x, y parms for x and y points in the surface */
int surf_x, surf_y;

/* panel holding surface parameters */
GLUI_Panel *surfPanel = NULL;

/* hold the current system mode */
int mode = NEW_CURVE;

GLint windowWidth = 640;
GLint windowHeight = 480;

void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 640.0f, 0.0f, 480.0f, 2500.0f, -1000.0f);

	camera.reset();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	camera.applyTransform();

    if(curve) curve->draw();
    if(surface) surface->draw();

	glutSwapBuffers();
}

void mouseButton(GLint button, GLint state, GLint x, GLint y) {
	if(mode == CAMERA) {
		camera.mouse(button, state, x, y);
        glutPostRedisplay();
	} else if(GLUT_LEFT_BUTTON == button && GLUT_DOWN == state) {
		switch(mode) {
			case NEW_CURVE:
				if(curve) curve->addControlPoint(x, windowHeight-y);
				break;
			case NEW_SURFACE:
				break;
			case MODIFY:
				if(curve) curve->selectControlPoint(x, windowHeight-y);
				break;
		}
        glutPostRedisplay();
    }
}

void mouseMotion(GLint x, GLint y) {
	if(mode == CAMERA) {
		camera.motion(x, y);
        glutPostRedisplay();
	} else if(mode == MODIFY) {
        if(curve) curve->modifySelectedControlPoint(x, windowHeight-y);
        glutPostRedisplay();
    }
}

void reshape(int x, int y) {
	camera.reshape(x, y);
}

void clear() {
	camera.reset();
    if(curve) {
    	delete curve;
   		curve = NULL;
    }
    if(surface) {
    	delete surface;
 	  	surface = NULL;
    }
}

void exit0() {
    clear();
    exit(-1);
}

void gluiHandler(int id) {
    switch(id) {
		case NEW_CURVE:
            mode = NEW_CURVE;
            clear();
            curve = new BezierCurve();
			break;
		case NEW_SURFACE:
            mode = NEW_SURFACE;
            surfPanel->enable();
			break;
		case SURF_X:
			break;
		case SURF_Y:
			break;
		case SURF_OK:
			surfPanel->disable();
            clear();
			surface = new BezierSurface();
			glutPostRedisplay(); /* DELETE */
			break;
        case MODIFY:
            mode = MODIFY;
            break;
		case VIEW:
            mode = VIEW;
            break;
        case CAMERA:
        	mode = CAMERA;
        	break;
		case CLEAR:
			clear();
            glutPostRedisplay();
			break;
		case U:
			if(curve) curve->setTangentU(u);
			break;
		case V:
			break;
		case QUIT:
			exit0();
			break;
    }
}

void initGLUI(int window) {
	GLUI *gluiSide = GLUI_Master.create_glui_subwindow(window, GLUI_SUBWINDOW_RIGHT);
	gluiSide->set_main_gfx_window(window);

	gluiSide->add_button("New Curve",     NEW_CURVE,   gluiHandler);

	gluiSide->add_button("New Surface",   NEW_SURFACE, gluiHandler);
	surfPanel = gluiSide->add_panel("Surface Parms", GLUI_PANEL_EMBOSSED);
	GLUI_Spinner *xS = gluiSide->add_spinner_to_panel(surfPanel, "X Pts.", 
									GLUI_SPINNER_INT, &surf_x, SURF_X, gluiHandler);
	xS->set_int_limits(3, 10);
	GLUI_Spinner *yS = gluiSide->add_spinner_to_panel(surfPanel, "Y Pts.", 
									GLUI_SPINNER_INT, &surf_y, SURF_Y, gluiHandler);
	yS->set_int_limits(3, 10);
	gluiSide->add_button_to_panel(surfPanel, "Ok", SURF_OK, gluiHandler);
	surfPanel->disable();

	gluiSide->add_button("Modify Points", MODIFY,      gluiHandler);
	gluiSide->add_button("Clear Screen",  CLEAR,       gluiHandler);
	gluiSide->add_button("Camera Mode",   CAMERA,      gluiHandler);

	gluiSide->add_separator();

	GLUI_Spinner *uS = gluiSide->add_spinner("U", GLUI_SPINNER_FLOAT, &u, U, gluiHandler);
	uS->set_float_limits(0.0f, 1.0f);
	uS->set_speed(50);
	GLUI_Spinner *vS = gluiSide->add_spinner("V", GLUI_SPINNER_FLOAT, &v, V, gluiHandler);
	vS->set_float_limits(0.0f, 1.0f);
	vS->set_speed(50);

	gluiSide->add_separator();

	gluiSide->add_button("Quit", QUIT, gluiHandler);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(-1, -1);
    int wnd = glutCreateWindow("Bezier Curve and Surface");

	glutDisplayFunc(display);
	glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);

	init();
	initGLUI(wnd);
	GLUI_Master.set_glutReshapeFunc(reshape);

	camera.init(GLUT_LEFT_BUTTON);

    glutMainLoop();
    
    return 0;
}
