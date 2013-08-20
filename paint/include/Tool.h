#ifndef TOOL_H_
#define TOOL_H_

#include <vector>

#include <glut.h>

#include "structs.h"

#include "Paintable.h"
#include "ToolListener.h"

/**
 * A tool defines something that can create paintables.  It is controlled by mouse input passed
 * to it, and in order, a mouse down begins creation, move augments it and up completes the
 * paintable creation.
 * 
 * Events are fired during augmentation (intermediaPaintableCreated) and upon completion of 
 * the paintable (finalPaintableCreated).
 */
class Tool {
	public:
		Tool();
		virtual ~Tool() {}

		/** Mouse handlers */
		virtual void mouseDown(struct point2f p);
		virtual void mouseMove(struct point2f p);
		virtual void mouseUp(struct point2f p);

		bool isMouseCurrentlyDown() { return mouseCurrentlyDown; }

		/** Add a listener to this tool */
		void addToolListener(ToolListener* t) { toolListeners.push_back(t); }

		/** Set/get color information for tools to be created */
		static struct color4f getColor() { return color; }
		static void setColor(GLfloat r, GLfloat g, GLfloat b);
		static void setColor(struct color4f color) { Tool::color = color; }

		/** get/set point size */
		static GLfloat getPointSize() { return size; }
		static void setPointSize(GLfloat size) { Tool::size = size; }

		/** get/set line width */
		static GLfloat getLineWidth() { return width; }
		static void setLineWidth(GLfloat width) { Tool::width = width; }

		/** get/set fill status */
		static bool isFill() { return fill; }
		static void setFill(bool fill) { Tool::fill = fill; }

	protected:
	
		/** Publish ToolListener events */
		void notifyIntermediatePaintableCreated(Paintable* p);
		void notifyFinalPaintableCreated(Paintable* p);

	private:
		bool mouseCurrentlyDown;

		std::vector< ToolListener* > toolListeners;

		static struct color4f color;
		static GLfloat size;
		static GLfloat width;
		static bool fill;
};

#endif /*TOOL_H_*/
