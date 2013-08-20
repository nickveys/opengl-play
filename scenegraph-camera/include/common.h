#ifndef COMMON_H_
#define COMMON_H_

#include <math.h>

typedef enum {
	ROTATE, ZOOM, TWIST, PAN, VIEW_ALL
} camera_mode_t;

#define DEG_RAD (180.0 / M_PI)

#define LOG(level, str)	printf("[%s] %s\n", level, str)
#define INFO(str)		LOG("INFO", str)
#define WARNING(str)	LOG("WARNING", str)
#define ERROR(str)		LOG("ERROR", str)

typedef struct {
	float x, y, z;
} point_3d_t;

typedef struct {
	point_3d_t min;
	point_3d_t max;
} bounds_t;

#endif /*COMMON_H_*/
