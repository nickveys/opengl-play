#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "structs.h"

#include "Paintable.h"

/**
 * Extension of Paintable describing a rectangle defined by an x0y0 and x1y1 pair.  The rectangle
 * can either be filled or just show the border.
 */
class Rectangle : public Paintable {
	public:
		Rectangle(bool filled);
		~Rectangle() {}

		/** control start/end points */
		void setStart(struct point2f start) { this->start = start; }
		void setEnd(struct point2f end) { this->end = end; }

		void paint();

	private:
		struct point2f start, end;
		bool filled;
};

#endif /*RECTANGLE_H_*/
