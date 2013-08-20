#ifndef ROOT_H_
#define ROOT_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glui.h>

#include "Camera.h"

/*
 * Unique implementation of Group.  This class is a singleton meant to be the sole root
 * of the scene graph.  In addition to simply being a group, it maintains which object
 * is the 'current' object.  This is the object currently being manipulated by the ui.
 */
class Root : public Group {

public:

	Root() {
		axisScale = 1.0; // larger than normal, this is the 'world'
		current = NULL;
		openGroup = NULL;

		min.x = min.y = min.z = -1.0f;
		max.x = max.y = max.z = 1.0f;
	}

	~Root() {
		if(openGroup) delete openGroup;
	}

	// add the current object (if any) to the open group, if no group
	// current exists, one will be created
	void addToGroup() {
		if(!current) return;
		if(!openGroup) openGroup = new Group();

		Object* o = current;
		if(list.size() == 1) {
			setCurrent(NULL);
		} else {
			next();
		}

		removeObject(o);
		openGroup->addObject(o);
	}

	// close the open group if there is one
	void closeGroup() {
		if(!openGroup) return;
		addObject(openGroup);
		openGroup = NULL;
	}

	void reopenGroup() {
		if(openGroup) return;
		if(!current) return;

		if(typeid(*current) == typeid(Group)) {
			openGroup = (Group*) current;
			next();
			removeObject(openGroup);
		}
	}

	void addObject(Object *r) {
		Group::addObject(r);

		// set current whenever something is added to the root
		setCurrent(r);
	}

	// select the next object to be active, wrap around if needed
	void next() {
		if(list.size() <= 1) return; // no 'next' to select
		
		// find current, increment list to next, unless at end
		for(unsigned int i = 0; i < list.size(); i++) {
			if(list[i] == current) {
				if(i == list.size() - 1)
					setCurrent(list[0]);
				else
					setCurrent(list[i+1]);
				break;
			}
		}
	}

	// set the given object to be the new current, also de-select the previous
	// current object, tie up new object to UI controls, and sync
	void setCurrent(Object* r) {
		// Unselect the old current
		if(current) current->setSelected(false);
    
		// Remember the new current.  If its null, we're done
		current = r;
		if(!r) return;

		// If current is not null, set it selected
		current->setSelected(true);

		// Update the interface to match the current object
		for(int i = 0; i < 3; i++) {
			obj_rdelta[i]       = 0;
			obj_scale[i]        = current->scale[i];
			obj_xlate[i]        = current->xlate[i];
			previousRotation[i] = current->rot[i];
		}
		GLUI_Master.sync_live_all(); // sync ui w/vars
	}

	// called when a UI widget manipulating the current object had changed
	void xformHandler() {
		if(!current) return;

		// Getting all the values and storing them in the 
		//   current object ensures we get whichever 
		//   one changed to generate this callback
		for(int i=0; i<3; i++) {
			current->scale[i] = obj_scale[i];
			current->xlate[i] = obj_xlate[i];
      
			// This is the ugly business needed to make the glui slider
			// work as an incremental value instead of an absolute value.
			current->rot[i] = obj_rdelta[i] * 90 - previousRotation[i];
			previousRotation[i] = obj_rdelta[i] * 90;
		}
		GLUI_Master.sync_live_all();

		// Compose the incremental rotation into the current object's rotation matrix
		current->composeRotation();
	}

	void reshape(int x, int y) {
		int tx, ty, tw, th;
		GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
		glViewport( tx, ty, tw, th );
		xy_aspect = (float)tw / (float)th;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, xy_aspect, 1.0, 128.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		GLfloat light_position[4] = { 1.0, 1.0, 1.0, 0.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}

	void display(Camera& camera) {
		// Clear the frame
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set the view based on the matrix in the glui interface
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		camera.applyTransform();

		draw();

		// If there is an open group, draw it
		if(openGroup) openGroup->draw();
    
		// Swap the buffers
		glutSwapBuffers(); 
	}

	virtual void printSceneGraph(int *tab) {
        cout << "Current Scene Graph" << endl << "Root" << endl;
		std::vector< Object* >::const_iterator itr = list.begin();
		while(itr != list.end()) {
			(*itr)->printSceneGraph(tab);
			itr++;
		}
	}

	// Interface connections...
	static float obj_scale[3];
	static float obj_xlate[3];
	static float previousRotation[3];
	static float obj_rdelta[3];

	// points to the current object. Null if there isn't one.
	Object* current;
 
	// points to the current group. Null if there isn't one.
	Group* openGroup;
 
	// aspect ratio of the window. learned in resize, used to set projection.
	float xy_aspect;
};

// Defintion of the root singleton and all of the Root classes static members.
Root    root;
float   Root::obj_scale[]  = { 1.0, 1.0, 1.0};
float   Root::obj_xlate[]  = { 0.0, 0.0, 0.0 };
float   Root::obj_rdelta[] = { 0.0, 0.0, 0.0 };
float   Root::previousRotation[] = { 0.0, 0.0, 0.0 };

#endif /*ROOT_H_*/
