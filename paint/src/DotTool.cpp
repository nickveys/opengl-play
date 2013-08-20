#include <stdio.h>

#include "DotTool.h"

DotTool::DotTool(bool single) : Tool() {
	dots = NULL;
	this->single = single;
}

void DotTool::mouseDown(struct point2f p) {
	Tool::mouseDown(p);
	dots = new Dots();
	dots->setColor(getColor());
	dots->setPointSize(getPointSize());
	dots->addPoint(p.x, p.y);
	notifyIntermediatePaintableCreated(dots);
}

void DotTool::mouseMove(struct point2f p) {
	if(isMouseCurrentlyDown() && !single) {
		dots->addPoint(p.x, p.y);
		notifyIntermediatePaintableCreated(dots);
	}
}

void DotTool::mouseUp(struct point2f p) {
	Tool::mouseUp(p);
	if(!single) {
		dots->addPoint(p.x, p.y);
	}
	notifyFinalPaintableCreated(dots);
	dots = NULL;
}
