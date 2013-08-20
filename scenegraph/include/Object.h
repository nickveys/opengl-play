#ifndef OBJECT_H_
#define OBJECT_H_

#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

/*
 * Object is a base class for all drawable objects.
 */
class Object {

public:

	// initialize all member variables
	Object() : rotate(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1), selected(0) {
		scale[0] = scale[1] = scale[2] = 1.0;
		xlate[0] = xlate[1] = xlate[2] = 0.0;
		rot[0]   = rot[1]   = rot[2]   = 0.0;
		setColor(1.0, 0.0, 1.0);
		axisScale = 0.5;
	}

	// destructor does nothing
	virtual ~Object() {}

	// tell an object what color it is
	void setColor(GLfloat r, GLfloat g, GLfloat b) {
		col[0] = r; col[1] = g; col[2] = b;
	}

	// tell an object if it is selected
	virtual void setSelected(bool s) { selected = s; }  

	// compose the current rot[3] matrix with the rotate matrix to form final rotation
	void composeRotation() {
		glPushMatrix();
			glLoadIdentity();

			// put new x,y,and z components of "rotation" together on the stack
			glMultMatrixf(rotate.m);
			glRotated(rot[0],1,0,0);
			glRotated(rot[1],0,1,0);
			glRotated(rot[2],0,0,1); 

			// update our rotation matrix
			glGetFloatv(GL_MODELVIEW_MATRIX, rotate.m);
			rot[0] = rot[1] = rot[2] = 0.0;
		glPopMatrix();
	}

	// draw the object.  this method handles the common transformations for all objects
	// and delegates actual drawing to the typeSpecificDraw method.
	void draw() {
		glPushMatrix();

			// do the transformations common to each object
			glTranslatef( xlate[0], xlate[1], xlate[2] ); 
			glMultMatrixf(rotate.m);
			glScalef(scale[0], scale[1], scale[2]);

			// draw the object's axes
			draw_axes(axisScale);

			if(selected) glColor3f(0.0, 0.0, 0.0);
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
			glutWireCone(0.04, 0.2, 12, 9);
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
	Matrix rotate;
  
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

protected:
	// method for subclasses to draw themselves
	virtual void typeSpecificDraw() = 0;

	// true if the object is selected
	bool selected;
};

/*** Specific Objects ***/

class Cone : public Object {

    void typeSpecificDraw() {
        glutWireCone(0.2, 0.7, 12, 9);
    }

    virtual void printSceneGraph(int *tab) {
		(*tab)++;
		for(int i = 0; i < (*tab); i++) cout << "\t";
		cout << "Cone" << endl;
		(*tab)--;
    }
};

class Sphere : public Object {

    void typeSpecificDraw() {
		glutWireSphere(0.2, 16, 16);
    }

    virtual void printSceneGraph(int *tab) {
		(*tab)++;
		for(int i = 0; i < (*tab); i++) cout << "\t";
		cout << "Sphere" << endl;
		(*tab)--;
    }
};

/*
 * A spindle generated via a gaussian curve, credited to 
 * "surfaceofrevolution" example and edited to fit
 */
class Spindle : public Object {

	double spindle(double x) {
		return 0.5 * exp(-20 * x * x) ;
	}

	void typeSpecificDraw() {
		double xStart = -0.45;
		double xStop  =  0.45;
		int nIntervalsX     = 16;
		int nIntervalsTheta = 16;
		const double PI = 3.14159;
		const double DX = ( xStop - xStart ) / nIntervalsX ;    // step size along x
		const double DTHETA = 2*PI / nIntervalsTheta ;         // step size around circle of revolution
 
		// initialize values - first slice.
		double x = xStart ;
		double r = spindle( xStart ) ;

		// draw wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glPushMatrix();
		glScalef(0.65, 0.65, 0.65); // draw just a bit smaller

		// Loop through the slices.
		for (int ix = 0 ; ix < nIntervalsX ; ++ix) {
			double xNext = x + DX ;
			double rNext = spindle(xNext) ;

			// Build a quadstrip around this slice.
			glBegin(GL_QUAD_STRIP);

				for (int ia = 0; ia <= nIntervalsTheta; ++ia) {

					// Update the angle as we move around the circle. 
					double theta = ia * DTHETA ;
					double c = cos( theta ) ;
					double s = sin( theta ) ;

					// Get the points on the circle at x and xNext.
					double y = r * c ;          double z = r * s ;
					double yNext = rNext * c ;  double zNext = rNext * s ;

					// Add edge at this angle from x to xNext (one boundary of quad strip).
					glVertex3d(     x,     y,     z ) ;
					glVertex3d( xNext, yNext, zNext ) ;
				}

				// Prepare for next pass through the x loop
				x = xNext ; 
				r = rNext ;

			glEnd() ; 
		}

		glPopMatrix();

		// back to solid mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	virtual void printSceneGraph(int *tab) {
		(*tab)++;
		for(int i = 0; i < (*tab); i++) cout << "\t";
		cout << "Spindle" << endl;
		(*tab)--;
	}
};

class Teapot : public Object {

    void typeSpecificDraw() {
        glutWireTeapot(0.2);
    }
    
    virtual void printSceneGraph(int *tab) {
		(*tab)++;
		for(int i = 0; i < (*tab); i++) cout << "\t";
		cout << "Teapot" << endl;
		(*tab)--;
    }
};

class Torus : public Object {

	void typeSpecificDraw() {
		glutWireTorus(0.10, 0.2, 16, 16);
	}

    virtual void printSceneGraph(int *tab) {
		(*tab)++;
		for(int i = 0; i < (*tab); i++) cout << "\t";
		cout << "Torus" << endl;
		(*tab)--;
    }
};

#endif /*OBJECT_H_*/
