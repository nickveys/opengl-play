#ifndef LINETOOL_H_
#define LINETOOL_H_

#include "structs.h"

#include "Line.h"
#include "Tool.h"

/**
 * A tool to create a line paintable.
 */
class LineTool : public Tool {
	public:
		LineTool();
		virtual ~LineTool() {}

		void mouseDown(struct point2f p);
		void mouseMove(struct point2f p);
		void mouseUp(struct point2f p);

	private:
		Line* line;
};

#endif /*LINETOOL_H_*/
