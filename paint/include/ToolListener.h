#ifndef TOOLLISTENER_H_
#define TOOLLISTENER_H_

#include "Paintable.h"

class ToolListener {
	public:
		virtual void intermediatePaintableCreated(Paintable* p) = 0;
		virtual void finalPaintableCreated(Paintable* p) = 0;
};

#endif /*TOOLLISTENER_H_*/
