// three_broken_circles_rotate.cpp
// Draw three rotating broken (dashed) circles with background (GLUT)

#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Parameters
int segments = 120;
float baseRadius = 0.5f;
float lineWidth = 2.0f;
float angle = 0.0f; // rotation angle

// Dash/gap settings
int dash1 = 6, gap1 = 4;
int dash2 = 8, gap2 = 2;
int dash3 = 3, gap3 = 5;

// Draw one broken circle
void drawBrokenCircle(float radius, int dash, int gap, float r, float g, float b)
{
    glColor3f(r, g, b);
    glLineWidth(lineWidth);

    for (int i = 0; i < segments; ++i)
    {
        int cycleLen = dash + gap;
        if (cycleLen <= 0) cycleLen = 1;
        int phase = i % cycleLen;
        if (phase >= dash) continue;

        float t1 = (float)i / segments;
        float t2 = (float)(i + 1) / segments;

        float angle1 = t1 * 2.0f * (float)M_PI;
        float angle2 = t2 * 2.0f * (float)M_PI;

        float x1 = radius * cosf(angle1);
        float y1 = radius * sinf(angle1);
        float x2 = radius * cosf(angle2);
        float y2 = radius * sinf(angle2);

        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }
}

void display()
{
    // Background color
    glClearColor(0.08f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Apply rotation
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    // Draw circles
    drawBrokenCircle(baseRadius, dash1, gap1, 1.0f, 0.3f, 0.3f);       // red
    drawBrokenCircle(baseRadius * 0.7f, dash2, gap2, 0.3f, 1.0f, 0.5f); // green
    drawBrokenCircle(baseRadius * 1.3f, dash3, gap3, 0.3f, 0.6f, 1.0f); // blue

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (w <= h)
        glOrtho(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w,
                1.0 * (GLfloat)h / (GLfloat)w, -1.0, 1.0);
    else
        glOrtho(-1.0 * (GLfloat)w / (GLfloat)h,
                1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0, -1.0, 1.0);
}

// Animate rotation
void idle()
{
    angle += 0.2f;
    if (angle > 360.0f)
        angle -= 360.0f;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27 || key == 'q' || key == 'Q')
        std::exit(0);

    switch (key)
    {
    case '+': segments += 12; break;
    case '-': if (segments > 12) segments -= 12; break;
    case 'r': baseRadius += 0.05f; break;
    case 'f': baseRadius -= 0.05f; if (baseRadius < 0.1f) baseRadius = 0.1f; break;
    case 'w': lineWidth += 0.5f; break;
    case 's': if (lineWidth > 0.5f) lineWidth -= 0.5f; break;
    }

    printf("segments=%d baseRadius=%.2f lineWidth=%.1f angle=%.1f\n",
           segments, baseRadius, lineWidth, angle);
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Rotating Broken Circles with Background");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle); // for animation

    printf("Controls:\n");
    printf("  + / - : increase / decrease number of segments\n");
    printf("  r / f : increase / decrease base radius\n");
    printf("  w / s : increase / decrease line width\n");
    printf("  q or ESC : quit\n");

    glutMainLoop();
    return 0;
}
