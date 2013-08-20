#ifndef BEZIERSURFACE_H_
#define BEZIERSURFACE_H_

/* max # of points supported */
#define BS_MAX_PTS_X 10
#define BS_MAX_PTS_Y 10

/* total # of line segments for the curve */
#define BS_MAX_SEGMENTS 50

typedef struct {
	float x, y, z;
} point3d_t;

class BezierSurface {
public:
    BezierSurface();
    ~BezierSurface();

    void draw();

protected:

    void allBernsteinU(float u, int countOffset = 0);
    void allBernsteinV(float v, int countOffset = 0);
    point3d_t getPoint(float u, float v);

    int countHoriz, countVert;

    float X[BS_MAX_PTS_X][BS_MAX_PTS_Y];
    float Y[BS_MAX_PTS_X][BS_MAX_PTS_Y];
    float Z[BS_MAX_PTS_X][BS_MAX_PTS_Y];
    float Bn[BS_MAX_PTS_X];
    float Bm[BS_MAX_PTS_Y];
};

#endif /*BEZIERSURFACE_H_*/
