#ifndef OBJECT_H_
#define OBJECT_H_

#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

#include "eigen/matrix.h"
using namespace Eigen;

#include "common.h"

/*
 * Object is a base class for all drawable objects.
 */
class Object {

public:

	// initialize all member variables
	Object() : selected(0) {
		scale[0] = scale[1] = scale[2] = 1.0;
		xlate[0] = xlate[1] = xlate[2] = 0.0;
		rot[0]   = rot[1]   = rot[2]   = 0.0;
		setColor(1.0, 0.0, 1.0);
		axisScale = 0.5;

		rotate.loadIdentity();

		min.x = min.y = min.z = -0.5f;
		max.x = max.y = max.z = 0.5f;
	}

	// destructor does nothing
	virtual ~Object() {}

	// tell an object what color it is
	void setColor(GLfloat r, GLfloat g, GLfloat b) {
		col[0] = r; col[1] = g; col[2] = b;
	}

	// tell an object if it is selected
	virtual void setSelected(bool s) { selected = s; }  

	virtual bounds_t getBounds() {
		bounds_t retval;

		glPushMatrix();
			glLoadIdentity();
			glTranslatef( xlate[0], xlate[1], xlate[2] ); 
			glMultMatrixf(rotate.array());
			glScalef(scale[0], scale[1], scale[2]);

			GLfloat minf[] = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  min.x, min.y, min.z, 1};
			glMultMatrixf(minf);

			glGetFloatv(GL_MODELVIEW_MATRIX, minf);
			retval.min.x = minf[12];
			retval.min.y = minf[13];
			retval.min.z = minf[14];

			glLoadIdentity();
			glTranslatef( xlate[0], xlate[1], xlate[2] ); 
			glMultMatrixf(rotate.array());
			glScalef(scale[0], scale[1], scale[2]);

			GLfloat maxf[] = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  max.x, max.y, max.z, 1};
			glMultMatrixf(maxf);

			glGetFloatv(GL_MODELVIEW_MATRIX, maxf);
			retval.max.x = maxf[12];
			retval.max.y = maxf[13];
			retval.max.z = maxf[14];
		glPopMatrix();

		return retval;
	}

	// compose the current rot[3] matrix with the rotate matrix to form final rotation
	void composeRotation() {
		glPushMatrix();
			glLoadIdentity();

			// put new x,y,and z components of "rotation" together on the stack
			glMultMatrixf(rotate.array());
			glRotated(rot[0],1,0,0);
			glRotated(rot[1],0,1,0);
			glRotated(rot[2],0,0,1); 

			// update our rotation matrix
			glGetFloatv(GL_MODELVIEW_MATRIX, rotate.array());
			rot[0] = rot[1] = rot[2] = 0.0;
		glPopMatrix();
	}

	// draw the object.  this method handles the common transformations for all objects
	// and delegates actual drawing to the typeSpecificDraw method.
	void draw() {
		glPushMatrix();

			// do the transformations common to each object
			glTranslatef( xlate[0], xlate[1], xlate[2] ); 
			glMultMatrixf(rotate.array());
			glScalef(scale[0], scale[1], scale[2]);

			// draw the object's axes
			draw_axes(axisScale);

			if(selected) glColor3f(0.3f, 0.8f, 0.5f);
			else glColor3f(col[0], col[1], col[2]);

			// draw specific object
			typeSpecificDraw();

		glPopMatrix();
	}

	// draw an arrow of given length up the z axis
	static void axis(double length) {
		glPushMatrix();
			glBegin(GL_LINES);
				glVertex3d(0, 0, 0);
				glVertex3d(0, 0, length);
			glEnd();
			glTranslated(0, 0, length - 0.2); 
			glutSolidCone(0.04, 0.2, 12, 9);
		glPopMatrix();
	}	

	// draw the x,y,z axis scaled by the given factor
	static void draw_axes(float scale) {
		glPushMatrix();
			glScalef(scale, scale, scale);

			glColor3f(0.0, 0.0, 1.0);  // z
			axis(1.0);

			glRotated(90.0, 0, 1, 0);  // x
			glColor3f(1.0, 0.0, 0.0);
			axis(1.0);

			glRotated(-90.0, 1, 0, 0); // y
			glColor3f(0.0, 1.0, 0.0);
			axis(1.0);
		glPopMatrix();
	}

	// print the current scene graph from this object's POV
	virtual void printSceneGraph(int *tab) = 0;

	// rotation matrix
	Matrix<GLfloat, 4> rotate;
  
	// x, y and z scale factors
	float scale[3];

	// x, y and z translation factors
	float xlate[3];

	// incremental rotation storage
	float rot[3];

	// the object's color.
	float col[3];

	// scale of the axis relative to the object
	float axisScale;

	// min/max coordinates for this object (unscaled/translated)
	point_3d_t min, max;

protected:
	// method for subclasses to draw themselves
	virtual void typeSpecificDraw() = 0;

	// true if the object is selected
	bool selected;
};

/*** Specific Objects ***/

class Cone : public Object {

public:
	Cone() {
		/* TODO init min/max */
	}

    virtual void printSceneGraph(int *tab) {
		(*tab)++;
		for(int i = 0; i < (*tab); i++) cout << "\t";
		cout << "Cone" << endl;
		(*tab)--;
    }

protected:
    void typeSpecificDraw() {
        glutSolidCone(0.2, 0.7, 12, 9);
    }
};

class Sphere : public Object {

public:
	Sphere() {
		/* TODO init min/max */
	}

    virtual void printSceneGraph(int *tab) {
		(*tab)++;
		for(int i = 0; i < (*tab); i++) cout << "\t";
		cout << "Sphere" << endl;
		(*tab)--;
    }

protected:
    void typeSpecificDraw() {
		glutSolidSphere(0.2, 16, 16);
    }
};

class Teapot : public Object {

public:
	Teapot() {
		/* TODO init min/max */
	}
    
    virtual void printSceneGraph(int *tab) {
		(*tab)++;
		for(int i = 0; i < (*tab); i++) cout << "\t";
		cout << "Teapot" << endl;
		(*tab)--;
    }

protected:
    void typeSpecificDraw() {
        glutSolidTeapot(0.2);
    }
};

class Torus : public Object {

public:
	Torus() {
		/* TODO init min/max */
	}

    virtual void printSceneGraph(int *tab) {
		(*tab)++;
		for(int i = 0; i < (*tab); i++) cout << "\t";
		cout << "Torus" << endl;
		(*tab)--;
    }

protected:
	void typeSpecificDraw() {
		glutSolidTorus(0.10, 0.2, 16, 16);
	}
};

#endif /*OBJECT_H_*/
