#ifndef BEZIERCURVE_H_
#define BEZIERCURVE_H_

/* max # of points supported */
#define BC_MAX_PTS 100

/* total # of line segments for the curve */
#define BC_MAX_SEGMENTS 50

typedef struct {
	float x, y;
} point2d_t;

class BezierCurve {
public:
    BezierCurve();
    ~BezierCurve();

    void addControlPoint(int x, int y);
    void selectControlPoint(int x, int y);
    void modifySelectedControlPoint(int x, int y);
    void setTangentU(float u) { tangentU = u; }

    void draw();

protected:

    void allBernstein(float u, int countOffset = 0);
    point2d_t getPoint(float u);
    point2d_t getTangent();

    int count;
    int selected;
    float tangentU;

    float X[BC_MAX_PTS];
    float Y[BC_MAX_PTS];
    float B[BC_MAX_PTS];
};

#endif /*BEZIERCURVE_H_*/
