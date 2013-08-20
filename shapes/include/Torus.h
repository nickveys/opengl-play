#ifndef TORUS_H_
#define TORUS_H_

#include <glut.h>

#include "Model.h"

/**
 * Torus model.
 */
class Torus : public Model {
	public:
		Torus() : Model() {}
		virtual ~Torus() {}
		void drawSelf()  { glutWireTorus(0.4f, 0.9f, 20, 25); }
};

#endif /*TORUS_H_*/
