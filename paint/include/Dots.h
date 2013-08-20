#ifndef DOTS_H_
#define DOTS_H_

#include <vector>

#include <glut.h>

#include "structs.h"

#include "Paintable.h"

/**
 * A collection of dots to be painted.  Can be used for one or many dots together.
 */
class Dots : public Paintable {
	public:
		Dots() : Paintable() {}
		~Dots();

		/** add a point to the vector of dots */
		void addPoint(GLfloat x, GLfloat y);

		void paint();

	private:
		std::vector< struct point2f* > dots;
};

#endif /*DOTS_H_*/
