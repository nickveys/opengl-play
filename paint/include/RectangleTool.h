#ifndef RECTANGLETOOL_H_
#define RECTANGLETOOL_H_

#include "structs.h"

#include "Rectangle.h"
#include "Tool.h"

/**
 * A tool used to create a filled or non-filled rectangle paintable 
 */
class RectangleTool : public Tool {
	public:
		RectangleTool(bool filled);
		virtual ~RectangleTool() {}

		void mouseDown(struct point2f p);
		void mouseMove(struct point2f p);
		void mouseUp(struct point2f p);

	private:
		Rectangle* rect;
		bool filled;
};

#endif /*RECTANGLETOOL_H_*/
