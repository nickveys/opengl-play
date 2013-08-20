#include <glut.h>

#include "Model.h"

Model::Model() : rotation(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1) {
	trans[0] = trans[1] = trans[2] = 0.0f;
	scale[0] = scale[1] = scale[2] = 1.0f;
	drawingAxis = true;
}

void Model::draw() {
	glPushMatrix();
		glTranslatef(trans[0], trans[1], trans[2]);
		glMultMatrixf(rotation.m);
		if(drawingAxis) {
			drawAxis(0.5f);
		}
		glScalef(scale[0], scale[1], scale[2]);
		setColor();
		drawSelf(); // virtual
	glPopMatrix();
}

void Model::drawAxis(GLfloat scale) {
	/* 
	 * walk through each axis rotating appropriately to point
	 * the line + arrow + label in the right direction
	 */
	glPushMatrix();
		glScalef(scale, scale, scale);

		glPushMatrix();
			glColor3f(1.0f, 0.0f, 0.0f);
			glRotatef(90, 0.0f, 1.0f, 0.0f);
			drawSingleAxis('x');
		glPopMatrix();

		glPushMatrix();
			glColor3f(0.0f, 1.0f, 0.0f);
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			drawSingleAxis('y');
		glPopMatrix();

		glPushMatrix();
			glColor3f(0.0f, 0.0f, 1.0f);
			drawSingleAxis('z');
		glPopMatrix();
	glPopMatrix();
}

void Model::drawSingleAxis(char label) {

	/* always assume we're on Z */
	glBegin(GL_LINES);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 0, 4);
	glEnd();

	/* apply text label to axis */
	glRasterPos3f(0.0f, 0.0f, 4.75f);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, label);

	/* axis tip */
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 4.0f);
		glutWireCone(0.1f, 0.5f, 10, 10);
	glPopMatrix();
}

void Model::setColor() {
	switch(color) {
		case YELLOW: glColor3f(1.0f, 1.0f, 0.0f); break;
		case PURPLE: glColor3f(0.5f, 0.0f, 1.0f); break;
		case ORANGE: glColor3f(1.0f, 0.6f, 0.0f); break;
		case WHITE:  glColor3f(1.0f, 1.0f, 1.0f); break;
	}
}
