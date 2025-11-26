#include <GL/freeglut.h>
#include <iostream>
using namespace std;

void display(){
    glClear(GL_COLOR_BUFFER_BIT); //clears the screen
    glBegin(GL_TRIANGLES); //start drawing triangle
    glColor3f(0.8f, 0.8f, 0.8f); //background color of the triangle

    //2d vertex (x, y)
    glVertex2f(0.0f, 0.5f); //top
    glVertex2f(-0.5f, -0.5f); //bottom-left
    glVertex2f(0.5f, -0.5f); //bottom-right

    glEnd(); //end drawing
    glutSwapBuffers(); //display the rendered image using double buffering
}

int main(int argc, char** argv){
    glutInit(&argc, argv); //initialize glut environment
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //set display mode
    glutInitWindowSize(800, 600); //set window size 
    glutCreateWindow("practice no.2"); //creates window with title
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //set window color
    glutDisplayFunc(display); //call display function to output inside window
    glutMainLoop(); //keeps window open
    return 0;
}