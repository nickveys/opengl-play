#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <glut.h>

/*
 * Some commonly used defines and structs
 */

#define min(x, y) x < y ? x : y
#define max(x, y) x > y ? x : y

struct point2f {
	GLfloat x, y;
};

struct color4f {
	GLfloat r, g, b, a;
};

#endif /*STRUCTS_H_*/
