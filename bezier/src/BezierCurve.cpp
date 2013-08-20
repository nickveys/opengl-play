#include <stdlib.h>
#include <stdio.h>

#include <GL/glut.h>

#include "BezierCurve.h"

BezierCurve::BezierCurve() {
    count = 0;
    selected = -1;
    tangentU = -1;
}

BezierCurve::~BezierCurve() {
}

void BezierCurve::addControlPoint(int x, int y) {
	printf("%d,%d\n",x,y);
    if (count < BC_MAX_PTS) {
        X[count] = (float) x;
        Y[count] = (float) y;
        count++;
    } else {
    	// WARN("Max points reached.");
    }
}

void BezierCurve::draw() {
    // Draw control points
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize( 10 );
    glBegin(GL_POINTS);
    for(int i = 0; i < count; i++) {
        glVertex2f(X[i], Y[i]);
    }
    glEnd();

    // Draw lines between control points
    glColor3f(0.0f, 0.0f, 1.0f);
    glLineWidth(1);
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < count; i++) {
        glVertex2f(X[i], Y[i]);
    }
    glEnd();

    // Draw curve
    if (count > 2) {
        glLineWidth(2);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_LINE_STRIP);

        for(int i = 0; i <= BC_MAX_SEGMENTS; i++) {
            point2d_t pt = getPoint((1.0f * i) / BC_MAX_SEGMENTS);
            glVertex2f(pt.x, pt.y);
        }

        glEnd();
    }

	// Draw tangent and normal lines
	if(tangentU >= 0) {
		point2d_t pt = getPoint(tangentU);
		point2d_t tan = getTangent();

		// tangent, point + vector
		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
			glVertex2f(pt.x, pt.y);
			glVertex2f(pt.x + tan.x, pt.y + tan.y);
		glEnd();

		// normal -> point + perp vector
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_LINE_STRIP);
			glVertex2f(pt.x, pt.y);
			glVertex2f(pt.x - tan.y, pt.y + tan.x);
		glEnd();
	}
}

void BezierCurve::selectControlPoint(int x, int y) {
    selected = -1;
    for(int i = 0; i < count; i++) {
        int deltaX = x - X[i];
        int deltaY = y - Y[i];
        if (deltaX < 0) deltaX = -deltaX;
        if (deltaY < 0) deltaY = -deltaY;

        // Selection with a reasonable range
        if (deltaX < 8 && deltaY < 8) {
            selected = i;
            break;
        }
    }
}

void BezierCurve::modifySelectedControlPoint(int x, int y) {
    if (selected != -1) {
        X[selected] = x;
        Y[selected] = y;
    }
}

void BezierCurve::allBernstein(float u, int countOffset) {
    /* Compute all n-th degree Bernstein Polynomials */
    /* Input: n, u */
    /* Output: B (an array, B[0], �K, B[n]) */
    int k = 0;
    float u1 = 1.0 - u;
    float saved = 0.0f;
    float temp = 0.0f;
    B[0] = 1.0;

    for(int j = 1; j < count + countOffset; j++) {
        saved = 0.0;
        for(k = 0; k < j; k++) {
            temp = B[k];
            B[k] = saved + u1 * temp;
            saved = u * temp;
        }

        B[j] = saved;
    }
}

/* Compute point on Bezier curve */
point2d_t BezierCurve::getPoint(float u) {
    allBernstein(u); /* B is a local array */
    point2d_t c = { 0.0f, 0.0f };
    for(int k = 0; k < count; k++) {
    	c.x += B[k] * X[k];
    	c.y += B[k] * Y[k];
    }
    return c;
}

point2d_t BezierCurve::getTangent() {
	allBernstein(tangentU, -1);
	point2d_t t = { 0.0f, 0.0f };
	int n = count - 1;
	for(int k = 0; k < n; k++) {
		t.x += B[k] * (X[k + 1] - X[k]);
		t.y += B[k] * (Y[k + 1] - Y[k]);
	}
	return t;
}
