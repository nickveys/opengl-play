#ifndef PAINTABLE_H_
#define PAINTABLE_H_

#include <glut.h>

#include "structs.h"

/**
 * A paintable describes something that can be displayed on the screen.  Paintables maintain 
 * their color, size and geometry.
 */
class Paintable {
	public:
		Paintable() : dead(false) { }

		virtual ~Paintable() {}

		/** set/get current paint color */
		inline struct color4f getColor() { return color; }
		inline void setColor(GLfloat r, GLfloat g, GLfloat b) { color.r = r; color.g = g; color.b = b; }
		inline void setColor(struct color4f color) { this->color = color; }

		/** set/get current point size */
		inline GLfloat getPointSize() { return size; }
		inline void setPointSize(GLfloat size) { this->size = size; }

		/** set/get current line width */
		inline GLfloat getLineWidth() { return width; }
		inline void setLineWidth(GLfloat width) { this->width = width; }

		/** kill or determine if this paintable is killed.  can be used to prune out the canvas */
		inline bool isDead() { return dead; }
		inline void kill() { dead = true; }

		/** implemented by paintables to draw themselves */
		virtual void paint() = 0;

	private:
		struct color4f color;
		GLfloat size;
		GLfloat width;
		bool dead;
};

#endif /*PAINTABLE_H_*/
