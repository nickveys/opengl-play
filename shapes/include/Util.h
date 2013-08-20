#ifndef UTIL_H_
#define UTIL_H_

#include <gl.h>
#include <glut.h>

/**
 * Utility class.
 */
class Util {
	public:
		/** Draw the vendor, renderer and version strings onscreen */
		static void drawStrings(char* vendor, char* renderer, char* version) {

			/*
			 * Set up a 2D ortho projection to draw easier
			 */
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(0, 1, 0, 1);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			drawString(vendor,   0.055f);
			drawString(renderer, 0.030f);
			drawString(version,  0.005f);
		}

	private:
		/* Draw a single ***NULL-terminated*** string @ [0,Y] */
		static void drawString(char* str, GLfloat y) {
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			int i = 0;
			char c = str[i];
			glRasterPos2f(0.005f, y);
			while(c) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);
				c = str[++i];
			}
		}
};

#endif /*UTIL_H_*/
