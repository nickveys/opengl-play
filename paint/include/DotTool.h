#ifndef DOTTOOL_H_
#define DOTTOOL_H_

#include "structs.h"

#include "Dots.h"
#include "Tool.h"

/**
 * A tool to create one or many dots in a single paintable.
 */
class DotTool : public Tool {
	public:
		DotTool(bool single);
		virtual ~DotTool() {}

		void mouseDown(struct point2f p);
		void mouseMove(struct point2f p);
		void mouseUp(struct point2f p);

	private:
		Dots* dots;
		bool single;
};

#endif /*DOTTOOL_H_*/
