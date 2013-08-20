#include "RectangleTool.h"

RectangleTool::RectangleTool(bool filled) {
	rect = NULL;
	this->filled = filled;
}

void RectangleTool::mouseDown(struct point2f p) {
	Tool::mouseDown(p);
	rect = new Rectangle(filled);
	rect->setColor(getColor());
	rect->setLineWidth(getLineWidth());
	rect->setStart(p);
	rect->setEnd(p);
	notifyIntermediatePaintableCreated(rect);
}

void RectangleTool::mouseMove(struct point2f p) {
	if(isMouseCurrentlyDown()) {
		rect->setEnd(p);
		notifyIntermediatePaintableCreated(rect);
	}
}

void RectangleTool::mouseUp(struct point2f p) {
	Tool::mouseUp(p);
	rect->setEnd(p);
	notifyFinalPaintableCreated(rect);
}
