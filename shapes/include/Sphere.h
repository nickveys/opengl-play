#ifndef SPHERE_H_
#define SPHERE_H_

#include <glut.h>

#include "Model.h"

/**
 * Sphere model.
 */
class Sphere : public Model {
	public:
		Sphere() : Model() {}
		virtual ~Sphere() {}
		void drawSelf()  { glutWireSphere(1.25f, 20, 20); }
};

#endif /*SPHERE_H_*/
