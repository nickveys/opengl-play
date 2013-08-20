#ifndef CANVAS_H_
#define CANVAS_H_

#include <vector>

#include "Paintable.h"

#define WORLD_X0 0.0f
#define WORLD_X1 1280.0f
#define WORLD_Y0 0.0f
#define WORLD_Y1 1024.0f

/**
 * Defines the canvas on which paintable things are placed and drawn.
 */
class Canvas {
	public:
		Canvas();
		~Canvas();

		/**
		 * Add a paintable object to the canvas.
		 */
		void addPaintable(Paintable* p);

		/**
		 * Set the paintable to be drawn as the "active" paintable.  This is reserved for
		 * the object currently being created on-screen.
		 */
		void setActivePaintable(Paintable* p);

		/**
		 * Remove all paintables from the canvas.
		 */
		void clear();

		/**
		 * Control fade-out of paintables.  All paintables will have their alpha channel
		 * dropped from 1.0 to 0.0, at which time they will be removed from the canvas.
		 * 
		 * @param on if true, enable fading, otherwise disable fading
		 */
		void enableFade(bool on) { this->fading = on; }

		/**
		 * Determine if the canvas is fading currently.
		 * 
		 * @return true if fading, false otherwise
		 */
		bool isFading() { return this->fading; }

		/**
		 * Advance animations one tick.
		 */
		void tick();

		/**
		 * Draw the canvas with its paintables.
		 */
		void drawScene();

	private:

		/**
		 * Remove all paintables from the canvas.
		 */
		void clearPaintables();

		std::vector< Paintable* > paintables;
		Paintable* activePaintable;
		bool fading;
};

#endif /*CANVAS_H_*/
