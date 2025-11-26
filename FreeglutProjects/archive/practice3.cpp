#include <GL/freeglut.h>
#include <iostream>
using namespace std;

float angle = 0.0f;

void display(){
    glClear(GL_COLOR_BUFFER_BIT); //clears the screen
    glLoadIdentity(); //reset transformations

    glPushMatrix(); //saves transformations
    glRotatef(angle, 0.0f, 0.0f, 1.0f); //rotate based on angle

    glBegin(GL_TRIANGLES); //start drawing triangle
    glColor3f(0.8f, 0.8f, 0.8f); //background color of the triangle

    //2d vertex (x, y)
    glVertex2f(0.0f, 0.5f); //top
    glVertex2f(-0.5f, -0.5f); //bottom-left
    glVertex2f(0.5f, -0.5f); //bottom-right

    glEnd(); //end drawing
    glPopMatrix(); //restores transformations
    glutSwapBuffers(); //display the rendered image using double buffering
}

void angleUpdate(int value){
    angle += 1.0f; //increment rotation angle

    //keep angle in range 
    if(angle > 360.0f){
        angle -= 360.0f; 
    }

    glutPostRedisplay(); //trigger Redraw
    glutTimerFunc(16, angleUpdate, 0); // 60 FPS
}

int main(int argc, char** argv){
    glutInit(&argc, argv); //initialize glut environment
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //set display mode
    glutInitWindowSize(800, 600); //set window size
    glutCreateWindow("practice no.2"); //create window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //set window color
    glutDisplayFunc(display); //call display function
    glutTimerFunc(0, angleUpdate, 0); //start the animation timer
    glutMainLoop(); //keeps window running
    return 0;
}