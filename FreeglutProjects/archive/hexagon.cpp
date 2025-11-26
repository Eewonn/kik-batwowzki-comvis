#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <string>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Hexagon parameters
const float HEXAGON_RADIUS = 150.0f;
const float CENTER_X = WINDOW_WIDTH / 2.0f;
const float CENTER_Y = WINDOW_HEIGHT / 2.0f + 50.0f;

// Hexagon vertices
struct Point {
    float x, y;
};

Point hexagonVertices[6];

void calculateHexagonVertices() {
    for (int i = 0; i < 6; i++) {
        float angle = i * M_PI / 3.0f;
        hexagonVertices[i].x = CENTER_X + HEXAGON_RADIUS * cos(angle);
        hexagonVertices[i].y = CENTER_Y + HEXAGON_RADIUS * sin(angle);
    }
}

void renderText(float x, float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void renderTextCentered(float x, float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18) {
    float textWidth = 0;
    for (char c : text) {
        textWidth += glutBitmapWidth(font, c);
    }
    glRasterPos2f(x - textWidth / 2.0f, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void display() {
    // Set background color (dark blue)
    glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up 2D projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw filled hexagon (light green)
    glColor3f(0.5f, 0.8f, 0.5f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; i++) {
        glVertex2f(hexagonVertices[i].x, hexagonVertices[i].y);
    }
    glEnd();

    // Draw hexagon outline (darker green)
    glColor3f(0.2f, 0.5f, 0.2f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 6; i++) {
        glVertex2f(hexagonVertices[i].x, hexagonVertices[i].y);
    }
    glEnd();

    // Draw vertex labels (black text)
    glColor3f(0.0f, 0.0f, 0.0f);
    std::string labels[6] = {"P1", "P2", "P3", "P4", "P5", "P6"};
    
    for (int i = 0; i < 6; i++) {
        // Position labels slightly outside the vertices
        float angle = i * M_PI / 3.0f;
        float labelX = CENTER_X + (HEXAGON_RADIUS + 25) * cos(angle);
        float labelY = CENTER_Y + (HEXAGON_RADIUS + 25) * sin(angle);
        
        renderTextCentered(labelX, labelY, labels[i], GLUT_BITMAP_HELVETICA_12);
    }

    // Draw name inside hexagon (dark blue text)
    glColor3f(0.1f, 0.1f, 0.6f);
    renderTextCentered(CENTER_X, CENTER_Y + 10, "Mark Eron Diaz", GLUT_BITMAP_TIMES_ROMAN_24);

    // Draw subject and section below hexagon (white text)
    glColor3f(1.0f, 1.0f, 1.0f);
    renderTextCentered(CENTER_X, CENTER_Y - HEXAGON_RADIUS - 50, "CS0045", GLUT_BITMAP_HELVETICA_18);
    renderTextCentered(CENTER_X, CENTER_Y - HEXAGON_RADIUS - 80, "Section: TN32", GLUT_BITMAP_HELVETICA_18);

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hexagon - Mark Eron Diaz");

    calculateHexagonVertices();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}