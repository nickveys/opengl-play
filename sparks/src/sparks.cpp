#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glut.h>

/* escape key for keyboard func */
#define KEY_ESC 27

#define SHOWFPS 1
#define TITLEBASE "Sparks!"
#define TWOPI (2.0 * 3.14159f)

/* initial physical settings */
#define SPARK_MAX_SIZE_INITIAL 4
#define SPARK_MAX_AGE_INITIAL 80
#define SPARKS_INITIAL 20
#define YG_INITIAL 0.0055f

/* world x/y bounds */
#define x0 0.0f
#define x1 1000.0f
#define y0 0.0f
#define y1 1000.0f

/* wrapper around rand() to provide min/max bounds */
#define boundedRandom(min, max) ((max - min) * (float) rand() / RAND_MAX) + min

/* assigns the timer function to updatePositions */
/* 1000 * 1.0 / (FRAMERATE) */
#define setTimerFunc() glutTimerFunc((int) (1000 * (1.0f / 60.0f)), updatePositions, 0)

/* frees point and node */
#define freeNode(n) free(n->p); free(n)

/* stores application data */
struct settings {
	int sparks, maxAge, maxSize;
	float yg;
} settings;

enum {
	CLEAR_POINTS, RESET_SETTINGS
};

/* stores a rgb color */
struct color {
	GLfloat r, g, b;
};

/* models a single point in the world */
typedef struct {
	/* model attributes */
	float age;
	GLfloat x,y;	/* positions */
	float vx0, vy0;	/* velocities */

	/* visual attributes */
	float size;
	struct color color;
} point_t;

/* singly linked-list structure to hold points */
struct node {
	point_t* p;
	struct node* next;
};

/* the points */
struct points {
	struct node* head;
	struct node* tail;
} points;

/*
 * calculate frames-per-second
 */
inline void fps() {
    static int frame = 0;
    static int timebase = 0;

	int time = glutGet(GLUT_ELAPSED_TIME);
    frame++;
	if (time - timebase > 1000) {
		char title[20];
		sprintf(title, "%s - %3.2ffps", TITLEBASE, frame*1000.0/(time-timebase));
		glutSetWindowTitle(title);
		timebase = time;		
		frame = 0;
	}
}

/* 
 * update point positions, called by timer
 */
void updatePositions(int timerCallbackValue) { 
	struct node* prev = NULL;
	struct node* cur = points.head;
	while(cur != NULL) 	{
		point_t* p = cur->p;

		/* are we off the world or 'dead'? */
		if(p->y <= 0.0 || p->age >= settings.maxAge) {
			/* save head pointer */
			struct node* temp = cur;

			if(cur == points.head) {
				points.head = cur = cur->next;
			} else {
				prev->next = cur->next;
				cur = cur->next;
			}

			/* release */
			freeNode(temp);
		} else {
			/* update position */
			p->age++;
			p->x += p->vx0;
			p->y -= p->vy0 + 0.5f * settings.yg * p->age * p->age;

			prev = cur;
		    cur = cur->next;
		}
	}
	
	glutPostRedisplay();
	setTimerFunc(); 
}

void drawScene() {
    /* clear window */
    glClear(GL_COLOR_BUFFER_BIT);

	struct node* cur = points.head;
	while(cur != NULL) {
		point_t* p = cur->p;
		glPointSize(p->size);
		struct color c = p->color;
		float ageRatio = 1.0f - (p->age / settings.maxAge);
		glColor3f(c.r * ageRatio, c.g * ageRatio, c.b * ageRatio);
	    glBegin(GL_POINTS);
       		glVertex2f(p->x, p->y);
	    glEnd();
	    cur = cur->next;
	}

    /* flush GL buffers */
    glutSwapBuffers();

    if(SHOWFPS) fps();
}

void resize(int w, int h) {
    glViewport(0, 0, w, h);
}

/*
 * add a random amount of sparks at the given location
 * x/y are in view coords here, converted internally
 */
void addSpark(int x, int y) {
	const int width = glutGet(GLUT_WINDOW_WIDTH);
	const int height = glutGet(GLUT_WINDOW_HEIGHT);
	if(x < 0 || x >= width || y < 0 || y >= height) return;	

	/* convert view to world coordinates */
	x = (int) (x1 * (x / (float) width));
    y = (int) (y1 * (height - y) / height);
	
	/* number of sides of the regular polygon from which the x and y initial velocities
	 * will be selected.  the equations are influenced by those on pg. 111 in the Hill book */
	const int numSides = 10;

	/* x and y radii of the regular polygon */
	const float xr = 4.0f, yr = 5.0f;

	const int numSparks = (int) boundedRandom(1, settings.sparks);
	for(int i = 0; i < numSparks; i++) {
		/* allocate point and assign model attributes */
		point_t* p = (point_t*) malloc(sizeof(point_t));
		p->age = 0.0f;
		p->x = x;
		p->y = y;

		/* select a radius within the polygon */
		float radius = TWOPI * boundedRandom(1, numSides) / numSides;
		
		/* compute initial velocities */
		p->vx0 = (xr * boundedRandom(0.2f, 1.0f)) * cos(radius);
		p->vy0 = (yr * boundedRandom(0.2f, 1.0f)) * sin(radius) - 3.0f;

		/* assign view attributes */
		p->size = boundedRandom(1.0f, settings.maxSize);
		p->color.r = boundedRandom(0.5f, 1.0f);
		p->color.g = boundedRandom(0.5f, 1.0f);
		p->color.b = boundedRandom(0.5f, 1.0f);

		/* build ll node and attach the point */
		struct node* n = (struct node*) malloc(sizeof(struct node));
		n->p = p;
		n->next = NULL;
		if(points.head == NULL) {
			points.tail = points.head = n;
		} else {
			points.tail->next = n;
			points.tail = n;
		}
	}
}

void mouseClicked(int buttonNum, int state, int x, int y) {
	if(state == GLUT_DOWN) {
		addSpark(x, y);
	}
}

void mouseMoved(int x, int y) {
	addSpark(x, y);
}

/*
 * clear all currently displayed points 
 */
void clearPoints() {
	struct node* cur = points.head;
	while(cur != NULL) {
		/* store current, walk over and free */
		struct node* temp = cur;
		cur = cur->next;
		freeNode(temp);
	}

	points.head = points.tail = NULL;
}

/*
 * handle popup menu input 
 */
void handleMenu(int sel) {
	switch(sel) {
		case CLEAR_POINTS:
			clearPoints();
			break;
		case RESET_SETTINGS:
			settings.sparks = SPARKS_INITIAL;
			settings.maxAge = SPARK_MAX_AGE_INITIAL;
			settings.maxSize = SPARK_MAX_SIZE_INITIAL;
			settings.yg = YG_INITIAL;
			break;
	}
}

/*
 * A: More Sparks
 * Z: Less Sparks
 * S: More Gravity
 * X: Less Gravity
 */
void keyPress(unsigned char key, int x, int y) {
	switch(key) {
		case 'a':
		case 'A':
			settings.sparks++;
			printf("Sparks: %d\n", settings.sparks);
			break;
		case 'z':
		case 'Z':
			if(settings.sparks > 1) settings.sparks--;
			printf("Sparks: %d\n", settings.sparks);
			break;
		case 's':
		case 'S':
			settings.yg += settings.yg;
			printf("Gravity: %2.20f\n", settings.yg);
			break;
		case 'x':
		case 'X':
			settings.yg /= 2.0f;
			printf("Gravity: %2.20f\n", settings.yg);
			break;
		case 'd':
		case 'D':
			settings.maxSize++;
			printf("Max Size: %d\n", settings.maxSize);
			break;
		case 'c':
		case 'C':
			if(settings.maxSize > 1) settings.maxSize--;
			printf("Max Size: %d\n", settings.maxSize);
			break;
		case 'f':
		case 'F':
			settings.maxAge++;
			printf("Max Age: %d\n", settings.maxAge);
			break;
		case 'v':
		case 'V':
			if(settings.maxAge > 1) settings.maxAge--;
			printf("Max Age: %d\n", settings.maxAge);
			break;
			break;
		case KEY_ESC:
			exit(0);
			break;
		default:
			printf("Unknown key '%c', keys:\n", key);
			printf(" A: More Sparks\n");
			printf(" Z: Less Sparks\n");
			printf(" S: More Gravity\n");
			printf(" X: Less Gravity\n");
			printf(" D: Bigger Sparks\n");
			printf(" C: Smaller Sparks\n");
			printf(" F: Longer Life\n");
			printf(" V: Shorter Life\n");
	}
}

void init(int w, int h) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(x0, x1, y0, y1);
    glViewport(0, 0, w, h);

	/* fill in initial settings */
	settings.sparks = SPARKS_INITIAL;
	settings.maxAge = SPARK_MAX_AGE_INITIAL;
	settings.maxSize = SPARK_MAX_SIZE_INITIAL;
	settings.yg = YG_INITIAL;

	/* initialize the linked list */
	points.head = points.tail = NULL;

	/* utility popup */	
	glutCreateMenu(handleMenu);
	glutAddMenuEntry("Clear Points", CLEAR_POINTS);
	glutAddMenuEntry("Reset Settings", RESET_SETTINGS);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
	const int w = 640, h = 480;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(w, h);
    glutInitWindowPosition(-1, -1);
    glutCreateWindow(TITLEBASE);

    glutDisplayFunc(drawScene);
    setTimerFunc(); 
    glutReshapeFunc(resize);
    glutMouseFunc(mouseClicked);
    glutMotionFunc(mouseMoved);
    glutKeyboardFunc(keyPress);

    init(w, h);
    glutMainLoop();

    return 0;
}
