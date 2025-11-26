#include <GL/freeglut.h>
#include <iostream>
using namespace std;

void *font = GLUT_BITMAP_HELVETICA_18; //set font
char text[] = "Mark Eron"; //set text input

void renderText(float x, float y, char* text){
    glRasterPos2f(x, y); //set text position

    //loop through each character in the string until null
    for (char *c = text; *c != 0; c++) { 
        glutBitmapCharacter(font, *c); //render character
    }
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT); //clears the screen
    glLoadIdentity(); //reset transformations

    glColor3f(0.0f, 0.0f, 0.0f); //set text color to black
    renderText(-0.1f, 0.0f, text); //set text position

    glutSwapBuffers(); //double buffering
}

int main(int argc, char** argv){
    glutInit(&argc, argv); //initialize glut environment
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //set display mode
    glutInitWindowSize(800, 600); //set window size
    glutCreateWindow("pratice no.4"); //create window
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f); //set window color
    glutDisplayFunc(display); //set display function 
    glutMainLoop(); //keeps window running
    return 0;
}