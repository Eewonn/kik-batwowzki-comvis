#include <GL/freeglut.h>
#include <iostream>
using namespace std;

void display(){
    glClear(GL_COLOR_BUFFER_BIT); //clears the screen
    glBegin(GL_POLYGON); //start drawing
    glColor3f(0.0f, 1.0f, 0.0f); //glColor3f(r, g, b) - rgb color format

    //glVertex2f(x, y) - 2d vertex coordinates
    glVertex2f(-0.5f, 0.5f); //top-left
    glVertex2f(0.5f, 0.5f); //top-right
    glVertex2f(0.5f, -0.5f); //bottom-right
    glVertex2f(-0.5f, -0.5f); //bottom-left

    glEnd(); //end drawing
    glutSwapBuffers(); // display the rendered image using double buffering
}

int main(int argc, char** argv){
    glutInit(&argc, argv); //initialize glut environment
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //set display mode
    glutInitWindowSize(800, 600); //set window size
    glutCreateWindow("practice no.1"); //creates window with title
    glutDisplayFunc(display); //call display function to output inside window
    glutMainLoop(); //keeps window open
    return 0;
}
