#ifndef CONE_H_
#define CONE_H_

#include <glut.h>

#include "Model.h"

/**
 * Cone model.
 */
class Cone : public Model {
	public:
		Cone() : Model() {}
		virtual ~Cone() {}
		void drawSelf()  { glutWireCone(0.6f, 1.8f, 15, 10); }
};

#endif /*CONE_H_*/
