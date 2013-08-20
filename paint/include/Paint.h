#ifndef PAINT_H_
#define PAINT_H_

#include "Canvas.h"
#include "Tool.h"

#define WINDOW_INIT_WIDTH 800
#define WINDOW_INIT_HEIGHT 600
#define WINDOW_TITLE_BASE "SuperPaint!"

/* Menu Choices */
enum choice {
	/* tools */
	POINT, SCRIBBLE, LINE, RECT_FILL, RECT_NOFILL,

	/* colors */
	RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET, WHITE, BLACK,

	/* sizes */
	TWO_S, FOUR_S, EIGHT_S, SIXTEEN_S,

	/* widths */
	TWO_W, FOUR_W, EIGHT_W, SIXTEEN_W,

	/* commands */
	FADE, CLEAR, QUIT
};

/**
 * Paint is the main application class, its methods are called via the glut fuctions defined in
 * main.cpp
 */
class Paint : public ToolListener {
	public:
		Paint();
		~Paint();

		/** draw the scene */
		void drawScene();

		/* glut callbacks */
		void handleKeypress(unsigned char key, int x, int y);
		void handleMenu(int choice);
		void handleMouseClick(int buttonNum, int state, int x, int y);
		void handleMouseMotion(int x, int y);
		void handleResize(int w, int h);
		void handleTimer(int val);

		/* from ToolListener interface */
		void intermediatePaintableCreated(Paintable* p);
		void finalPaintableCreated(Paintable* p);

		/** execute the main loop */
		void loop();

	private:

		/** switch the current tool to the given tool */
		void switchTool(Tool* newTool);

		/** get a 2d point from a x, y pair in view coordinates */
		struct point2f pointFromViewXY(int x, int y);

		Canvas* canvas;
		Tool* currentTool;
};

#endif /*PAINT_H_*/
