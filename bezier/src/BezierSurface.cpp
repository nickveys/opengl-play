#include <stdlib.h>
#include <stdio.h>

#include <GL/glut.h>

#include "BezierSurface.h"

BezierSurface::BezierSurface() {
	countHoriz = countVert = 3;

	X[0][0] = 160;
	Y[0][0] = 360;
	Z[0][0] = 200;

	X[0][1] = 320;
	Y[0][1] = Y[0][0];
	Z[0][1] = 50;

	X[0][2] = 480;
	Y[0][2] = Y[0][0];
	Z[0][2] = 200;

	X[1][0] = X[0][0];
	Y[1][0] = 240;
	Z[1][0] = 100;

	X[1][1] = X[0][1];
	Y[1][1] = Y[1][0];
	Z[1][1] = 25;

	X[1][2] = X[0][2];
	Y[1][2] = Y[1][0];
	Z[1][2] = 100;

	X[2][0] = X[0][0];
	Y[2][0] = 120;
	Z[2][0] = -50;

	X[2][1] = X[0][1];
	Y[2][1] = Y[2][0];
	Z[2][1] = 50;

	X[2][2] = X[0][2];
	Y[2][2] = Y[2][0];
	Z[2][2] = -50;
}

BezierSurface::~BezierSurface() {
}

void BezierSurface::draw() {
    // Draw control points
    glPointSize(10);
    glColor3f(1.0f, 0.0f, 0.0f);
    for(int i = 0; i < countHoriz; i++) {
	    for(int j = 0; j < countVert; j++) {
		    glBegin(GL_POINTS);
		        glVertex3f(X[i][j], Y[i][j], Z[i][j]);
		    glEnd();
	    }
    }

    // Draw lines between control points
    glColor3f(0.0f, 0.0f, 1.0f);
    glLineWidth(1);
    for(int i = 0; i < countHoriz; i++) {
    	glBegin(GL_LINE_STRIP);
	    	for(int j = 0; j < countVert; j++) {
        		glVertex3f(X[i][j], Y[i][j], Z[i][j]);
		    }
	    glEnd();
	}

    // Draw surface
    if (countHoriz > 2 && countVert > 2) {
        glLineWidth(1);
        glColor3f(1.0f, 1.0f, 0.0f);

		for(int i = 0; i < BS_MAX_SEGMENTS; i++) {
       		glBegin(GL_QUAD_STRIP);
				float u = (1.0f * i) / BS_MAX_SEGMENTS;
				float uNext = (1.0f * (i+1)) / BS_MAX_SEGMENTS;
				for(int j = 0; j < BS_MAX_SEGMENTS; j++) {
					float v = (1.0f * j) / BS_MAX_SEGMENTS;
					point3d_t pt = getPoint(u, v);
					point3d_t ptNext = getPoint(uNext, v);
	
					glVertex3f(pt.x, pt.y, pt.z);
					glVertex3f(ptNext.x, ptNext.y, ptNext.z);
				}
       		glEnd();
		}
    }
}

/* Compute all n-th degree Bernstein Polynomials */
void BezierSurface::allBernsteinU(float u, int countOffset) {
    float u1 = 1.0 - u;
    float saved = 0.0f;
    float temp = 0.0f;
    Bn[0] = 1.0;

    for(int i = 1; i < countHoriz + countOffset; i++) {
        saved = 0.0;
        for(int k = 0; k < i; k++) {
            temp = Bn[k];
            Bn[k] = saved + u1 * temp;
            saved = u * temp;
        }

        Bn[i] = saved;
    }
}

/* Compute all n-th degree Bernstein Polynomials */
void BezierSurface::allBernsteinV(float v, int countOffset) {
    float v1 = 1.0 - v;
    float saved = 0.0f;
    float temp = 0.0f;
    Bm[0] = 1.0;

    for(int i = 1; i < countVert + countOffset; i++) {
        saved = 0.0;
        for(int k = 0; k < i; k++) {
            temp = Bm[k];
            Bm[k] = saved + v1 * temp;
            saved = v * temp;
        }

        Bm[i] = saved;
    }
}

/* Compute point on Bezier surface */
point3d_t BezierSurface::getPoint(float u, float v) {
    allBernsteinU(u);
    allBernsteinV(v);
    point3d_t c = { 0.0f, 0.0f, 0.0f };
    for(int i = 0; i < countHoriz; i++) {
    	for(int j = 0; j < countVert; j++) {
    		c.x += Bn[i] * Bm[j] * X[i][j];
    		c.y += Bn[i] * Bm[j] * Y[i][j];
    		c.z += Bn[i] * Bm[j] * Z[i][j];
    	}
    }
    return c;
}
