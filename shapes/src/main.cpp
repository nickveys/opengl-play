#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include <glut.h>
#include <glui.h>

#include "Matrix.h"
#include "Model.h"
#include "Cone.h"
#include "Cube.h"
#include "Sphere.h"
#include "Teapot.h"
#include "Torus.h"
#include "Util.h"

#include "enums.h"

#define TITLEBASE "Super 3D Viewer!"

struct {
	int spin, throb;
	color_t color;
	axis_drawing_t axisDraw;
} state;

// Glut main window handle
GLint mainWindowHandle;

// View Rotation Matrix
Matrix viewRotMatrix(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

// Object Rotation Matrix
Matrix currentRotation(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

// XYZ scaling inputs
GLfloat currentScale[3] = {1.0, 1.0, 1.0};

// XYZ translation inputs
GLfloat currentTrans[3] = {0.0, 0.0, 0.0};

// Slider rotation inputs
float currentSliderRotation[3];

// The currently manipulated model
Model* currentModel;

// All models
std::vector< Model* > models;

// Bottom panel to enable/disable on startup
GLUI *gluiBottom = NULL;

// GL_VENDOR, GL_RENDERER and GL_VERSION strings
char vendor[80], renderer[80], version[80];

/* gracefully exit after cleanup of dynamic vars */
void cleanExit() {
	/* current is in the vector as well */
	std::vector< Model* >::iterator itr = models.begin();
	while(itr != models.end()) {
		Model* m = *itr;
		itr = models.erase(itr);
		delete m;
	}
	currentModel = NULL;

	exit(0);
}

//////////////////////////////////////////////////////
// GlUI Callback Functions                          //
//////////////////////////////////////////////////////
void addObject(int objNum) {
	currentScale[0] = currentScale[1] = currentScale[2] = 1.0f;
	currentTrans[0] = currentTrans[1] = currentTrans[2] = 0.0f;
	currentRotation.identity();
	GLUI_Master.sync_live_all();

	if(currentModel == NULL) {
		// first add, enable the controls
		gluiBottom->enable();
	} else {
		if(state.axisDraw == CURRENT) {
			currentModel->setDrawingAxis(false);
		}
	}

	switch(objNum) {
		case CONE: 
			currentModel = new Cone();
			break;
		case CUBE:
			currentModel = new Cube();
			break;
		case SPHERE:
			currentModel = new Sphere();
			break;
		case TEAPOT: 
			currentModel = new Teapot();
			break;
		case TORUS:
			currentModel = new Torus();
			break;
	}
	models.push_back(currentModel);
	currentModel->setColor(state.color);
	currentModel->setDrawingAxis(state.axisDraw == CURRENT || state.axisDraw == ALL);
}

void rotateView(int dummy) {
	// do nothing
}

void rotateObject(int dummy) {
	if(currentModel != NULL) {
		currentModel->setRotation(currentRotation);
	}
}

void rotateObjectAxis(int index) {
	// keep track of the last increment
	static float lastSliderRotation[3] = { 0.0f, 0.0f, 0.0f };

	int scale = 90; // scale to make the movement more pronounced
	float thisRotation[3] = { 
		(currentSliderRotation[0] - lastSliderRotation[0]) * scale,
		(currentSliderRotation[1] - lastSliderRotation[1]) * scale,
		(currentSliderRotation[2] - lastSliderRotation[2]) * scale
	};

	lastSliderRotation[0] = currentSliderRotation[0];
	lastSliderRotation[1] = currentSliderRotation[1];
	lastSliderRotation[2] = currentSliderRotation[2];

	currentRotation.rotateBy(thisRotation[0], thisRotation[1], thisRotation[2]);

	if(currentModel != NULL) {
		currentModel->setRotation(currentRotation);
	}

	GLUI_Master.sync_live_all();
}

void scaleObject(int dummy) {
	if(currentModel != NULL) {
		currentModel->setScale(currentScale[0], currentScale[1], currentScale[2]);
	}
}

void translateObject(int dummy) {
	if(currentModel != NULL) {
		currentModel->setTranslation(currentTrans[0], currentTrans[1], currentTrans[2]);
	}
}

void color(int c) {
	if(currentModel != NULL) {
		currentModel->setColor(state.color);
	}
}

// handle turn on/off the axis displays
void axis(int a) {
	bool cur = true;	// current on/off
	bool all = false;	// all on/off
	switch(state.axisDraw) {
		case ALL:
			cur = all = true;
			break;
		case CURRENT:
			break;
		case NONE:
			cur = false;
			break;
	}

	std::vector< Model* >::const_iterator itr = models.begin();
	while(itr != models.end()) {
		(*itr)->setDrawingAxis(all);
		itr++;
	}
	if(currentModel != NULL) {
		currentModel->setDrawingAxis(cur);
	}
}

void spin(int dummy) {
	// do nothing
}

void throb(int dummy) {
	// do nothing
}

//////////////////////////////////////////////////////
//    GLUI initialization                           //
//////////////////////////////////////////////////////
void initGLUI(void) {

	/*
	 * Right Side Menu
	 */
	GLUI *gluiSide = GLUI_Master.create_glui_subwindow( mainWindowHandle, GLUI_SUBWINDOW_RIGHT );
	gluiSide->set_main_gfx_window( mainWindowHandle );

	/* Sub-panel of objects that can be added */
	GLUI_Panel* objPanel = gluiSide->add_panel("Add Objects");
	gluiSide->add_button_to_panel(objPanel, "Cone",   CONE,   (GLUI_Update_CB) addObject);
	gluiSide->add_button_to_panel(objPanel, "Cube",   CUBE,   (GLUI_Update_CB) addObject);
	gluiSide->add_button_to_panel(objPanel, "Sphere", SPHERE, (GLUI_Update_CB) addObject);
	gluiSide->add_button_to_panel(objPanel, "Teapot", TEAPOT, (GLUI_Update_CB) addObject);
	gluiSide->add_button_to_panel(objPanel, "Torus",  TORUS,  (GLUI_Update_CB) addObject);

	/* Manipulations on the current object */
	GLUI_Panel* curPanel = gluiSide->add_panel("Current Object");
	gluiSide->add_checkbox_to_panel(curPanel, "Spin", &(state.spin), 0, (GLUI_Update_CB) spin);
	gluiSide->add_checkbox_to_panel(curPanel, "Throb", &(state.throb), 0, (GLUI_Update_CB) throb);

	gluiSide->add_separator_to_panel(curPanel);

	GLUI_RadioGroup *colorGrp = gluiSide->add_radiogroup_to_panel(curPanel, (int*)&(state.color), 0, color);
	gluiSide->add_radiobutton_to_group(colorGrp, "Yellow");
	gluiSide->add_radiobutton_to_group(colorGrp, "Purple");
	gluiSide->add_radiobutton_to_group(colorGrp, "Orange");
	gluiSide->add_radiobutton_to_group(colorGrp, "White");

	/* Axis control */
	GLUI_Panel* axisPanel = gluiSide->add_panel("Draw Axis?");
	GLUI_RadioGroup *axisGrp = gluiSide->add_radiogroup_to_panel(axisPanel, (int*)&(state.axisDraw), 0, axis);
	gluiSide->add_radiobutton_to_group(axisGrp, "All");
	gluiSide->add_radiobutton_to_group(axisGrp, "Current");
	gluiSide->add_radiobutton_to_group(axisGrp, "None");

	gluiSide->add_button("Quit", 0, (GLUI_Update_CB) cleanExit);

	/*
	 * Bottom Menu
	 */
	gluiBottom = GLUI_Master.create_glui_subwindow(mainWindowHandle, GLUI_SUBWINDOW_BOTTOM);
	gluiBottom->set_main_gfx_window(mainWindowHandle);

	/* View and Object trackballs */
	GLUI_Rotation *view_rot = gluiBottom->add_rotation("View", viewRotMatrix.m, 0, rotateView );
	view_rot->set_spin(0.0);

	gluiBottom->add_column(false);
	GLUI_Rotation *obj_rot = gluiBottom->add_rotation("Object", currentRotation.m, 0, rotateObject );
	obj_rot->set_spin(0.0);

	/* XYZ Translation Sliders */
	gluiBottom->add_column(false);
	GLUI_Translation *trans_x = gluiBottom->add_translation("Translate X", GLUI_TRANSLATION_X, &currentTrans[0], 0, translateObject);
	trans_x->set_speed( .005 );

	gluiBottom->add_column(false);
	GLUI_Translation *trans_y = gluiBottom->add_translation("Translate Y", GLUI_TRANSLATION_Y, &currentTrans[1], 0, translateObject);
	trans_y->set_speed( .005 );

	gluiBottom->add_column(false);
	GLUI_Translation *trans_z = gluiBottom->add_translation("Translate Z", GLUI_TRANSLATION_Z, &currentTrans[2], 0, translateObject);
	trans_z->set_speed( .005 );

	/* XYZ Rotation Sliders */
	gluiBottom->add_column(false);
	GLUI_Translation *rot_x = gluiBottom->add_translation("Rotate X", GLUI_TRANSLATION_X, &currentSliderRotation[0], 0, rotateObjectAxis);
	rot_x->set_speed(.005);

	gluiBottom->add_column(false);
	GLUI_Translation *rot_y = gluiBottom->add_translation("Rotate Y", GLUI_TRANSLATION_Y, &currentSliderRotation[1], 0, rotateObjectAxis);
	rot_y->set_speed(.005);

	gluiBottom->add_column(false);
	GLUI_Translation *rot_z = gluiBottom->add_translation("Rotate Z", GLUI_TRANSLATION_Z, &currentSliderRotation[2], 0, rotateObjectAxis);
	rot_z->set_speed(.005);

	/* XYZ Scale Spinners */
	gluiBottom->add_column(false);
	gluiBottom->add_spinner("X Scale", GLUI_SPINNER_FLOAT, &currentScale[0], 0, scaleObject);
	gluiBottom->add_spinner("Y Scale", GLUI_SPINNER_FLOAT, &currentScale[1], 0, scaleObject);
	gluiBottom->add_spinner("Z Scale", GLUI_SPINNER_FLOAT, &currentScale[2], 0, scaleObject);

	// initially disabled as there are no objects
	gluiBottom->disable();
}

void reshape(int x, int y) {
	int tx, ty, tw, th;
	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glViewport( tx, ty, tw, th );
}

// calculate current frames per second
inline void fps() {
    static int frame = 0;
    static int timebase = 0;

	int time = glutGet(GLUT_ELAPSED_TIME);
    frame++;
	if (time - timebase > 1000) {
		char title[80];
		sprintf(title, "%s - %3.2ffps", TITLEBASE, frame*1000.0/(time-timebase));
		glutSetWindowTitle(title);
		timebase = time;		
		frame = 0;
	}
}

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	/* set up the world */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5, 5, -5, 5, 10, -10);

	/* look from (4,4,4) at (0,0,0) with <0,0,1> as UP */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(4, 4, 4, 0, 0, 0, 0, 0, 1);

	/* apply world view rotation */
	glMultMatrixf(viewRotMatrix.m);

	/* draw main world axis */
	Model::drawAxis();

	/* draw static models */
	std::vector< Model* >::const_iterator itr = models.begin();
	while(itr != models.end()) {
		(*itr)->draw();
		itr++;
	}

	/* draw the active model */
	if(currentModel != NULL) {
		currentModel->draw();
	}

	/* show gl strings */
	Util::drawStrings(vendor, renderer, version);

	glutSwapBuffers();

	fps();
}

// timer function tick rate
#define TICK_RATE 15

void handleTimer(int val) {
	static int scaleCount = 15;
	static char sign = 1;

	/* throb animation */
	if(state.throb && currentModel != NULL) {
		if(scaleCount == 0) {
			sign *= -1;
			scaleCount = 10;
		} else {
			scaleCount--;
		}

		currentModel->addScale(0.01f * sign);
	}

	/* spin animation */
	if(state.spin && currentModel != NULL) {
		currentModel->getRotation().rotateBy(1.0f, 2.0f, 3.0f);
	}

	if(currentModel != NULL) {
		currentRotation = currentModel->getRotation();
		GLUI_Master.sync_live_all();
		glutPostRedisplay();
	}
	glutTimerFunc(TICK_RATE, handleTimer, 0);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

	/* Initialize GLUT */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize(800, 600);
	glutTimerFunc(TICK_RATE, handleTimer, 0);
	mainWindowHandle = glutCreateWindow(TITLEBASE);
	glutDisplayFunc(display);

	state.spin = state.throb = 0;
	state.color = WHITE;
	state.axisDraw = CURRENT;

	/* Initialize GLUI */
	initGLUI();
	GLUI_Master.set_glutReshapeFunc(reshape);

	/* Get the needed strings */
	snprintf(vendor,   80, "  Vendor: %s", glGetString(GL_VENDOR));
	snprintf(renderer, 80, "Renderer: %s", glGetString(GL_RENDERER));
	snprintf(version,  80, " Version: %s", glGetString(GL_VERSION));

	glutMainLoop();

	return 0;
}
