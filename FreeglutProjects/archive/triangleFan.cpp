#include <GL/glut.h>
#include <cmath>
#include <string>
using namespace std;

const float PI = 3.14159265f; // For angle calculation
float rotationAngle = 0.0f; // Global rotation angle variable

// For text output
void renderText(float x, float y, const string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

// For display output of the triangle
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Save current matrix
    glPushMatrix();
    
    // Rotate by dynamic angle (counterclockwise)
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    
    // Draw colored triangle fan with smooth gradients
    glBegin(GL_TRIANGLE_FAN);
    
    // Center vertex color
    glColor3f(0.9f, 0.9f, 0.9f);
    glVertex2f(0.0f, 0.0f);
    
    // Create hexagon
    int sides = 6;
    float radius = 0.7f;
    
    // Create Rainbow Colors
    for (int i = 0; i <= sides; i++) {
        float angle = 2.0f * PI * i / sides;

        switch (i % 6) {
            case 0: 
                // Red
                glColor3f(1.0f, 0.0f, 0.0f); 
                break; 
            case 1: 
                // Yellow
                glColor3f(1.0f, 1.0f, 0.0f); 
                break; 
            case 2: 
                // Green
                glColor3f(0.0f, 1.0f, 0.0f); 
                break;
            case 3: 
                // Cyan
                glColor3f(0.0f, 1.0f, 1.0f); 
                break; 
            case 4: 
                // Blue
                glColor3f(0.0f, 0.0f, 1.0f); 
                break;
            case 5: 
                // Magenta
                glColor3f(1.0f, 0.0f, 1.0f); 
                break;
        }
        
        float xPos = radius * cos(angle);
        float yPos = radius * sin(angle);
        glVertex2f(xPos, yPos);
    }
    
    glEnd();
    
    // Create text in center (rotates with the triangle fan, perfectly centered)
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(-0.15f, -0.02f, "Mark Eron Diaz");
    
    glPopMatrix();
    
    // Create text below hexagon (stays fixed)
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(-0.25f, -0.9f, "CS0045 - TN32");
    
    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Set up orthographic projection
    if (width <= height) {
        glOrtho(-1.0, 1.0, -1.0 * (GLfloat)height / (GLfloat)width,
                1.0 * (GLfloat)height / (GLfloat)width, -1.0, 1.0);
    } else {
        glOrtho(-1.0 * (GLfloat)width / (GLfloat)height,
                1.0 * (GLfloat)width / (GLfloat)height, -1.0, 1.0, -1.0, 1.0);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Timer callback function for animation (60fps)
void timer(int value) {
    // Increment rotation angle for counterclockwise rotation
    rotationAngle += 2.0f; // Degrees per frame
    
    // Keep angle in range [0, 360)
    if (rotationAngle >= 360.0f) {
        rotationAngle -= 360.0f;
    }
    
    // Trigger redraw
    glutPostRedisplay();
    
    // Schedule next timer callback (1000ms / 60fps ≈ 16.67ms)
    glutTimerFunc(16, timer, 0);
}

void init() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH); 
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Triangle Fan Rotation - OpenGL");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    // Start the timer for 60fps animation
    glutTimerFunc(16, timer, 0);
    
    glutMainLoop();
    return 0;
}