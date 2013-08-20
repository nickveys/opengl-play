#ifndef LINE_H_
#define LINE_H_

#include "structs.h"

#include "Paintable.h"

/**
 * A line defined by a start and end point.
 */
class Line : public Paintable {
	public:
		Line();
		~Line() {}

		/** control start/end points */
		void setStart(struct point2f start) { this->start = start; }
		void setEnd(struct point2f end) { this->end = end; }

		void paint();

	private:
		struct point2f start, end;
};

#endif /*LINE_H_*/
