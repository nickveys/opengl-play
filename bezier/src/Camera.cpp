#include <math.h>
#include <assert.h>
#include <stdio.h>

#include <GL/glut.h>

#include "Camera.h"

Camera::Camera(void) {
	mode = ROTATE;
	moving = false;
	width = height = 1;
	currPos[0] = currPos[1] = prevPos[0] = prevPos[1] = 0.0f;
}

void Camera::init(GLuint button) {
	this->button = button;

	/* put the identity in the transforms */
	glPushMatrix();
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, translation);
		glGetFloatv(GL_MODELVIEW_MATRIX, rotation);
	glPopMatrix();
}

void Camera::reset() {
	/* put the identity in the transforms */
	glPushMatrix();
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, translation);
		glGetFloatv(GL_MODELVIEW_MATRIX, rotation);
	glPopMatrix();
}

void Camera::setMode(camera_mode_t mode) {
	this->mode = mode;
}

void Camera::applyTransform() {
	glMultMatrixf(translation);
	glMultMatrixf(rotation);
}

void Camera::reshape(int width, int height) {
	this->width  = width;
	this->height = height;
}

void Camera::mouse(int button, int state, int x, int y) {
	if(this->button == button) {
		if (state == GLUT_DOWN) {
			currPos[0] = (float) x / (float) width;
			currPos[1] = (float) y / (float) height;
			prevPos[0] = currPos[0];
			prevPos[1] = currPos[1];
			moving = true;
		} else if (state == GLUT_UP) {
			moving = false;
		}
	}
}

void Camera::motion(int x, int y) {
    if (!moving) return;

    currPos[0] = (float) x / (float) width;
    currPos[1] = (float) y / (float) height;

    float dx = currPos[0] - prevPos[0];
	float dy = currPos[1] - prevPos[1];
	float mag = sqrtf(dx * dx + dy * dy);

    if (mag < 1.0e-6f) return;

    switch (mode) {
        case ROTATE: rotate(dx, dy); break;
        case ZOOM:   zoom(dx, dy);   break;
        case TWIST:  twist(dx, dy);  break;
        case PAN:    pan(dx, dy);    break;
    }

    prevPos[0] = currPos[0];
    prevPos[1] = currPos[1];
}

void Camera::viewAll(bounds_t bounds) {
	/* transform the min/max to account for the camera position */
	glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(rotation);			
		
		GLfloat minf[] = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  bounds.xMin, bounds.yMin, bounds.zMin, 1};
		glMultMatrixf(minf);
			
		glGetFloatv(GL_MODELVIEW_MATRIX, minf);
		bounds.xMin = minf[12];
		bounds.yMin = minf[13];
		bounds.zMin = minf[14];

		glLoadIdentity();
		glMultMatrixf(rotation);

		GLfloat maxf[] = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  bounds.xMax, bounds.yMax, bounds.zMax, 1};
		glMultMatrixf(maxf);
			
		glGetFloatv(GL_MODELVIEW_MATRIX, maxf);
		bounds.xMax = maxf[12];
		bounds.yMax = maxf[13];
		bounds.zMax = maxf[14];
	glPopMatrix();

	/* get the max distance in the bounding box via pythagorean thm */
	double x1 = bounds.xMin - bounds.xMax;
	double y1 = bounds.yMin - bounds.yMax;
	double z1 = bounds.zMin - bounds.zMax;
	
	double x2 = x1 * x1;
	double y2 = y1 * y1;
	double z2 = z1 * z1;
	
	double hyp = sqrt(x2 + y2 + z2);

	/* find x, y look at position and z camera location */
	GLfloat x = (bounds.xMax + bounds.xMin) / 2.0f;
	GLfloat y = (bounds.yMax + bounds.yMin) / 2.0f;
	GLfloat z = (hyp / 2.0) / tan(30.0f / DEG_RAD);

	glPushMatrix();
		glLoadIdentity();
 		glTranslatef(-x, -y, -z);
 		glGetFloatv(GL_MODELVIEW_MATRIX, translation);
 	glPopMatrix();
}

void Camera::rotate(float dx, float dy) {
	float magnitude = sqrtf(dx * dx + dy * dy);
	_glRotatef(magnitude * 200.0f, dy, dx, 0.0f);
}

void Camera::zoom(float dx, float dy) {
	_glTranslatef(0.0f, 0.0f, -dy, 5.0f);
}

void Camera::twist(float dx, float dy) {
	_glRotatef(dx * 100.0f, 0.0f, 0.0f, 1.0f);
}

void Camera::pan(float dx, float dy) {
	_glTranslatef(dx, -dy, 0.0f, 3.0f);
}

void Camera::_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
	glPushMatrix();
		glLoadIdentity();
 		glRotatef(angle, x, y, z);
		glMultMatrixf(rotation);
 		glGetFloatv(GL_MODELVIEW_MATRIX, rotation);
 	glPopMatrix();
 }

void Camera::_glTranslatef(GLfloat x, GLfloat y, GLfloat z, float mag) {
	glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(translation);
 		glTranslatef(x * mag, y * mag, z * mag);
 		glGetFloatv(GL_MODELVIEW_MATRIX, translation);
 	glPopMatrix();
}
