#include <stdio.h>
#include <stdlib.h>
#include <glut.h>

typedef struct {
    GLfloat x, y;
} GLintPoint;

void myDisplay()
{
    /* clear window */
    glClear(GL_COLOR_BUFFER_BIT);

    GLintPoint T[3];
    T[0].x = -10;
    T[0].y = -10;
    T[1].x = 10;
    T[2].y = 10;
    T[1].y = -10;
    T[2].x = 0;

    int idx = rand() % 3;
    GLintPoint pt = T[idx];

    glPointSize(1.0);
    glBegin(GL_POINTS);
        glVertex2i(pt.x, pt.y);
        int i=0;
        for(; i<250000; i++) {
          idx = rand() % 3;
          pt.x = (pt.x + T[idx].x) / 2;
          pt.y = (pt.y + T[idx].y) / 2;
          glColor3f(1.0 * ((float)pt.x / 3), 1.0 * ((float)pt.y / 3), 1.0 * ((float)(3-pt.x) / 3));
          glVertex2f(pt.x, pt.y);
        }
    glEnd();

    glRotatef(1.0, 0.0, 0.0, 1.0);

    /* flush GL buffers */
    glutSwapBuffers();
}

void myReshape(int w, int h)
{
    printf("myReshape(%d, %d)\n", w, h);
    glViewport(0, 0, w, h);
}

void myMouse(int buttonNum, int state, int x, int y)
{
    printf("myMouse(%d, %d, %d, %d)\n", buttonNum, state, x, y);
}

void myMovedMouse(int x, int y)
{
    printf("myMovedMouse(%d, %d)\n", x, y);
}

void myKeyboard(unsigned char key, int x, int y)
{
    printf("myKeyboard(%c, %d, %d)\n", key, x, y);
}

void myInit(int w, int h)
{
    printf("myInit()\n");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
    glViewport(0, 0, w, h);
}

int main(int argc, char** argv) {
    const int w = 800;
    const int h = 600;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(w, h);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Go home Sierpinski, you're drunk");

    glutDisplayFunc(myDisplay);
    glutIdleFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glutMouseFunc(myMouse);
    glutMotionFunc(myMovedMouse);
    glutKeyboardFunc(myKeyboard);

    myInit(w, h);
    glutMainLoop();

    return 0;
}
