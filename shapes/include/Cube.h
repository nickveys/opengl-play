#ifndef CUBE_H_
#define CUBE_H_

#include <glut.h>

#include "Model.h"

/**
 * Cube model.
 */
class Cube : public Model {
	public:
		Cube() : Model() {}
		virtual ~Cube() {}
		void drawSelf() { glutWireCube(1.0f); }
};

#endif /*CUBE_H_*/
