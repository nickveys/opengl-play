#include <iostream>
using namespace std;

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glui.h>

#include "Matrix.h"
#include "Object.h"
#include "Group.h"
#include "Root.h"

///////////////////////////////////////////////
// GLUI callbacks  ... These all forward to the
//                     root object. The root is 
//                     a singleton instance of
//                     the Root class that is
//                     a kind of group which is
//                     itself a kind of Object.
void xformCallback(int code) {
  root.xformHandler();
}

#define TORUS_ID   1
#define TEAPOT_ID  2
#define CONE_ID    3
#define NEXT_ID    4
#define ADD_ID     5
#define CLOSE_ID   6
#define PRINT_ID   7
#define SPHERE_ID  8
#define REOPEN_ID  9
#define SPINDLE_ID 10

void control_cb( int control ) {
	int tab = 0;
	switch(control) {
		case TORUS_ID:
			root.addObject(new Torus());   break;
		case TEAPOT_ID:
			root.addObject(new Teapot());  break;
		case CONE_ID:
			root.addObject(new Cone());    break;
		case SPHERE_ID:
			root.addObject(new Sphere());  break;
		case SPINDLE_ID:
			root.addObject(new Spindle()); break;
		case NEXT_ID:
			root.next(); break;
		case ADD_ID:
			root.addToGroup(); break;
		case REOPEN_ID:
			root.reopenGroup(); break;
		case CLOSE_ID:
			root.closeGroup(); break;
		case PRINT_ID:
			root.printSceneGraph(&tab);
			break;
		case 0:
			// no global dynamically allocated objects to free
			exit(0);
	}
}

void reshape(int x, int y) {
  root.reshape(x, y);
}

void display() {
  root.display();
}

void glutAndGluiInit(int argc, char* argv[]) {
	//////////////////////////////////////////
	//   Initialize GLUT and create window  
	//////////////////////////////////////////
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize(640, 480);
 
	int main_window = glutCreateWindow("Homework 3 - Scenegraph");
	glutDisplayFunc(display);
	GLUI_Master.set_glutReshapeFunc(reshape);  

	//////////////////////////////////////////
	//         GLUI UI setup         
	//////////////////////////////////////////
	// Create the side subwindow 
	GLUI* glui = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_RIGHT );
	glui->set_main_gfx_window( main_window );

	////////// Buttons 
	GLUI_Panel* panel = glui->add_panel("Add Object");
	glui->add_button_to_panel(panel, "Cone",    CONE_ID,    control_cb);
	glui->add_button_to_panel(panel, "Sphere",  SPHERE_ID,  control_cb);
	glui->add_button_to_panel(panel, "Spindle", SPINDLE_ID, control_cb);
	glui->add_button_to_panel(panel, "Teapot",  TEAPOT_ID,  control_cb);
	glui->add_button_to_panel(panel, "Torus",   TORUS_ID,   control_cb);

	panel = glui->add_panel("Grouping");
	glui->add_button_to_panel(panel, "Add Selected", ADD_ID,    control_cb);
	glui->add_button_to_panel(panel, "Select Next",  NEXT_ID,   control_cb);
	glui->add_button_to_panel(panel, "Close Group",  CLOSE_ID,  control_cb);
	glui->add_button_to_panel(panel, "Reopen Group", REOPEN_ID, control_cb);

	panel = glui->add_panel("Other");
	glui->add_button_to_panel(panel, "Print Graph", PRINT_ID, control_cb);
	glui->add_button_to_panel(panel, "Quit",        0,        control_cb);

	///////////// Create the bottom subwindow 
	GLUI* glui2 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM );
	glui2->set_main_gfx_window( main_window );

	///////////// View rotation
	GLUI_Rotation *view_rot   = glui2->add_rotation( "View", &root.view_rotate.m[0]);
	view_rot->set_spin( 1.0 );  glui2->add_column( false );

	///////////// Object Translations
	GLUI_Translation *trans_x = glui2->add_translation("Xlate X", GLUI_TRANSLATION_X, &root.obj_xlate[0], 0, xformCallback);
	trans_x->set_speed( .005 ); glui2->add_column( false );
	GLUI_Translation *trans_y = glui2->add_translation("Xlate Y", GLUI_TRANSLATION_Y, &root.obj_xlate[1], 0, xformCallback);
	trans_y->set_speed( .005 ); glui2->add_column( false );
	GLUI_Translation *trans_z = glui2->add_translation("Xlate Z", GLUI_TRANSLATION_Z, &root.obj_xlate[2], 0, xformCallback);
	trans_z->set_speed( .005 ); glui2->add_column( false );

	///////////// Object Rotations
	GLUI_Translation *r_x = glui2->add_translation("Rotate X", GLUI_TRANSLATION_X, &root.obj_rdelta[0], 0, xformCallback);
	r_x->set_speed( .005 ); glui2->add_column( false );
	GLUI_Translation *r_y = glui2->add_translation("Rotate Y", GLUI_TRANSLATION_X, &root.obj_rdelta[1], 0, xformCallback);
	r_y->set_speed( .005 ); glui2->add_column( false );
	GLUI_Translation *r_z = glui2->add_translation("Rotate Z", GLUI_TRANSLATION_X, &root.obj_rdelta[2], 0, xformCallback);
	r_z->set_speed( .005 ); glui2->add_column( false );

	////////// Object Scale
	GLUI_Spinner *xspin = glui2->add_spinner("XScale:", GLUI_SPINNER_FLOAT, &root.obj_scale[0], 0, xformCallback);
	xspin->set_float_limits( .5f, 4.0 ); xspin->set_alignment( GLUI_ALIGN_RIGHT );
	GLUI_Spinner *yspin = glui2->add_spinner("YScale:", GLUI_SPINNER_FLOAT, &root.obj_scale[1], 0, xformCallback);
	yspin->set_float_limits( .5f, 4.0 ); yspin->set_alignment( GLUI_ALIGN_RIGHT );
	GLUI_Spinner *zspin = glui2->add_spinner("ZScale:", GLUI_SPINNER_FLOAT, &root.obj_scale[2], 0, xformCallback);
	zspin->set_float_limits( .5f, 4.0 ); zspin->set_alignment( GLUI_ALIGN_RIGHT );
}

int main(int argc, char* argv[]) {
	// Create the graphics window and the interface
	glutAndGluiInit(argc, argv);

	// Start the glut Event Loop
	glutMainLoop();

	return 0;
}
