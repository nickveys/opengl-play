#include "LineTool.h"

LineTool::LineTool() {
	line = NULL;
}

void LineTool::mouseDown(struct point2f p) {
	Tool::mouseDown(p);
	line = new Line();
	line->setColor(getColor());
	line->setLineWidth(getLineWidth());
	line->setStart(p);
	line->setEnd(p);
	notifyIntermediatePaintableCreated(line);
}

void LineTool::mouseMove(struct point2f p) {
	if(isMouseCurrentlyDown()) {
		line->setEnd(p);
		notifyIntermediatePaintableCreated(line);
	}
}

void LineTool::mouseUp(struct point2f p) {
	Tool::mouseUp(p);
	line->setEnd(p);
	notifyFinalPaintableCreated(line);
}
