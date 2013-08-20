#ifndef CAMERA_H_
#define CAMERA_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include "common.h"

/* 
 *  Simple trackball-like motion adapted from 
 *  "Mathematical Elements for Computer Graphics" by David F. Rogers &
 *  Alan J. Adams 1976. Section 3-8, pp. 55 ~ 59.
 *  Implemented by Tsung-Pin Yeh 10/26/2005. Modified 10/11/2006
*/

class Camera {
public:
    Camera(void);

/* functions */
    void setMode(camera_mode_t mode);
    void init(GLuint button);
    void applyTransform();
    void reshape(int width, int height);
    void mouse(int button, int state, int x, int y);
    void motion(int x, int y);
    void viewAll(bounds_t bounds);

protected:
    camera_mode_t mode;

    bool	moving;
    GLint	button;
    GLfloat	currPos[2], prevPos[2];
    GLfloat	translation[16], rotation[16];

    GLuint	width;
    GLuint	height;

    void rotate(float dx, float dy);
    void zoom(float dx, float dy);
    void twist(float dx, float dy);
    void pan(float dx, float dy);

	void _glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
	void _glTranslatef(GLfloat x, GLfloat y, GLfloat z, float mag);
};

#endif /*CAMERA_H_*/
