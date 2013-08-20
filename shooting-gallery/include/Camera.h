#ifndef CAMERA_H_
#define CAMERA_H_

#include <gl.h>

/*
 * Simple first-person camera
 */
class Camera {
public:
    Camera(void);

	/* apply modelview transformation */
    void apply();

	/* apply *only* the projection transformation */
    void applyProjection();

	/* handle reshaping of the viewport */
    void reshape(int _width, int _height);

	/* 
	 * strafe the given amount in world units
	 * negative # = left, positive # = right
	 */
    void strafe(float amount);

	/* 
	 * walk the given amount in world units
	 * negative # = forward, positive # = backward
	 */
    void walk(float amount);

	/* turn along the X axis by the given amount */
    void yaw(float amount);

	/* turn along the Y axis by the given amount */
    void pitch(float amount);

protected:
	/* x/y/z location and look-at points */
	GLfloat location[3], lookAt[3];

	/* viewport width and height */
	GLuint width, height;
};

#endif /*CAMERA_H_*/
