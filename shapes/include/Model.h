#ifndef MODEL_H_
#define MODEL_H_

#include <gl.h>

#include "enums.h"

#include "Matrix.h"

/**
 * Base class for all objects to be added to the scene.  This class
 * handles all transformation logic for the subclasses including scaling,
 * rotation and translation.  The extenders of this class need only implement
 * the drawSelf method supplying their drawing assuming nothing about their 
 * location on-screen.
 */
class Model {
	public:
		Model();
		virtual ~Model() {}

		/* Add given scale factor to current scaling, affects all axis */
		void addScale(GLfloat factor) { scale[0] += factor; scale[1] += factor; scale[2] += factor; }

		/* Get/set the current rotation matrix */
		Matrix& getRotation() { return rotation; }
		void setRotation(Matrix& m) { rotation = m; }

		/* Set scaling for each axis */
		void setScale(GLfloat x, GLfloat y, GLfloat z) 
			{ scale[0] = x; scale[1] = y; scale[2] = z; }

		/* Set translation for each axis */
		void setTranslation(GLfloat x, GLfloat y, GLfloat z) 
			{ trans[0] = x; trans[1] = y; trans[2] = z; }

		/* Set the drawing color */
		void setColor(color_t color) { this->color = color; }

		/* Set whether the XYZ axis should be drawn */
		void setDrawingAxis(bool draw) { this->drawingAxis = draw; }

		/* Draw the model */
		void draw();

		/* Draw the model geometry in a location-independent manner */
		virtual void drawSelf() = 0;

		/* Draw the XYZ axis (can be scaled by a %age) */
		static void drawAxis(GLfloat scale = 1.0f);

	private:
		Matrix rotation;	// rotation matrix
		GLfloat trans[3];	// xyz translations
		GLfloat scale[3];	// xyz scaling
		color_t color;		// draw color
		bool drawingAxis;	// whether axis are being drawn

		/* Draw a single axis arrow + label */
		static void drawSingleAxis(char label);

		/* Apply the model's color to the glColor */
		void setColor();
};

#endif /*MODEL_H_*/
