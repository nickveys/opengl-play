#ifndef TEAPOT_H_
#define TEAPOT_H_

#include <glut.h>

#include "Model.h"

/**
 * Teapot model.
 */
class Teapot : public Model {
	public:
		Teapot() : Model() {}
		virtual ~Teapot() {}
		void drawSelf()  { glutWireTeapot(1); }
};

#endif /*TEAPOT_H_*/
