/*
==============================================
===OpenGL 2D Face with Interactive Controls===
==============================================
===          VBO VERSION                   ===
==============================================

Keyboard Controls
- WASD: Move face around
- '+' / '-': Scale face up/down
- 'm': Toggle smile/frown
- 'b': Change background color randomly
- 'r': Reset face to default state
- 'ESC': Exit program

Mouse Controls
- Left click: Change left eye color randomly
- Right click: Change right eye color randomly
- Face automatically rotates continuously

==============================================
==============================================
 */

#include <GL/glew.h>   
#include <GL/glut.h>    
#include <cstdlib>     
#include <ctime>        
#include <cmath>      
#include <iostream>
#include <vector>    
using namespace std;

// Face Position
float faceX = 0.0f;     
float faceY = 0.0f;    

// Face transformation properties
float faceRotation = 0.0f; // Current Position
float faceScale = 1.0f; // Face Scale Variable
bool isSmiling = true; // Smiling Face Boolean Variable

// Eye color
float leftEyeColor[3];    
float rightEyeColor[3];     

// Background color
float backgroundColor[3] = {0.2f, 0.5f, 0.9f};

// VBO and VAO IDs
GLuint headVBO, headVAO;
GLuint leftEyeVBO, leftEyeVAO;
GLuint rightEyeVBO, rightEyeVAO;
GLuint mouthVBO, mouthVAO;

// Vertex data containers
vector<float> headVertices;
vector<float> leftEyeVertices;
vector<float> rightEyeVertices;
vector<float> mouthVertices;

// Forward declarations
void cleanup();


/*
===============
===FUNCTIONS===
===============
*/

// Generate a random color with random rgb values
void randomColor(float color[3]) {
    color[0] = (rand() % 100) / 100.0f; // Red Value
    color[1] = (rand() % 100) / 100.0f; // Green Value
    color[2] = (rand() % 100) / 100.0f; // Blue Value
}

// Generate circle vertices for VBO
void generateCircleVertices(vector<float>& vertices, float cx, float cy, float r, int num_segments) {
    vertices.clear();
    
    // Center vertex for triangle fan
    vertices.push_back(cx);
    vertices.push_back(cy);
    
    // Generate vertices around the circle
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta) + cx;
        float y = r * sinf(theta) + cy;
        
        vertices.push_back(x);
        vertices.push_back(y);
    }
}

// Generate mouth vertices based on smile/frown state
void generateMouthVertices(vector<float>& vertices, bool smiling) {
    vertices.clear();
    
    if (smiling) {
        // Generate smile curve
        for (float a = 0; a <= 3.14159f; a += 0.1f) { 
            vertices.push_back(0.2f * cosf(a));
            vertices.push_back(-0.2f - 0.1f * sinf(a));
        }
    } else {
        // Generate frown curve
        for (float a = 3.14159f; a >= 0; a -= 0.1f) {
            vertices.push_back(0.2f * cosf(a));
            vertices.push_back(-0.15f + 0.05f * sinf(a));
        }
    }
}

// Create and setup VBO for a given set of vertices
void setupVBO(GLuint& vbo, GLuint& vao, const vector<float>& vertices, GLenum drawMode = GL_TRIANGLE_FAN) {
    // Generate VAO and VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    // Bind VAO first
    glBindVertexArray(vao);
    
    // Bind and fill VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // Set up vertex attribute pointers
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Initialize all VBOs
void initializeVBOs() {
    // Generate head vertices
    generateCircleVertices(headVertices, 0.0f, 0.0f, 0.5f, 100);
    setupVBO(headVBO, headVAO, headVertices);
    
    // Generate left eye vertices
    generateCircleVertices(leftEyeVertices, -0.15f, 0.2f, 0.08f, 20);
    setupVBO(leftEyeVBO, leftEyeVAO, leftEyeVertices);
    
    // Generate right eye vertices
    generateCircleVertices(rightEyeVertices, 0.15f, 0.2f, 0.08f, 20);
    setupVBO(rightEyeVBO, rightEyeVAO, rightEyeVertices);
    
    // Generate mouth vertices
    generateMouthVertices(mouthVertices, isSmiling);
    setupVBO(mouthVBO, mouthVAO, mouthVertices, GL_LINE_STRIP);
}

// Update mouth VBO when smile state changes
void updateMouthVBO() {
    generateMouthVertices(mouthVertices, isSmiling);
    
    glBindBuffer(GL_ARRAY_BUFFER, mouthVBO);
    glBufferData(GL_ARRAY_BUFFER, mouthVertices.size() * sizeof(float), mouthVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Draw using VBO
void drawWithVBO(GLuint vao, int vertexCount, GLenum drawMode = GL_TRIANGLE_FAN) {
    glBindVertexArray(vao);
    glDrawArrays(drawMode, 0, vertexCount);
    glBindVertexArray(0);
}

// Draw the face with all transformations applied using VBOs
void drawFace() {
    glPushMatrix(); // Save transformations
    
    // Apply transformations
    glTranslatef(faceX, faceY, 0.0f); // Move face to current position
    glRotatef(faceRotation, 0.0f, 0.0f, 1.0f); // Rotate around Z-axis
    glScalef(faceScale, faceScale, 1.0f); // Scale in X and Y
    
    // Draw the head
    glColor3f(1.0f, 0.9f, 0.6f); // Set color to skin tone
    drawWithVBO(headVAO, headVertices.size() / 2, GL_TRIANGLE_FAN);

    // Draw left eye
    glColor3fv(leftEyeColor);  
    drawWithVBO(leftEyeVAO, leftEyeVertices.size() / 2, GL_TRIANGLE_FAN);

    // Draw right eye
    glColor3fv(rightEyeColor);
    drawWithVBO(rightEyeVAO, rightEyeVertices.size() / 2, GL_TRIANGLE_FAN);

    // Draw the mouth
    glColor3f(0.0f, 0.0f, 0.0f); // Set color to black
    glLineWidth(3.0f); // Make line thicker
    drawWithVBO(mouthVAO, mouthVertices.size() / 2, GL_LINE_STRIP);
    glLineWidth(1.0f); // Reset line width to default
    
    glPopMatrix(); // Reset transformations
}

// Redraw the screen
void display() {
    // Set the background color
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
    
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw face
    drawFace();
    glutSwapBuffers();
}

// Add timer to rotation
void timer(int value) {
    // Increment rotation angle
    faceRotation += 1.0f;
    
    // Keep rotation angle around 360 degrees
    if (faceRotation > 360.0f) {
        faceRotation = 0.0f;
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// Keyboard Controls
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    // ESC key (ASCII 27) to quit the program
    case 27:
        std::cout << "Program terminated by user.\n";
        cleanup(); // Clean up VBOs before exit
        exit(0);
        
    // Movement controls - WASD
    case 'w':
    case 'W':
        faceY += 0.05f; 
        break;
    case 's':
    case 'S':
        faceY -= 0.05f;
        break;
    case 'a':
    case 'A':
        faceX -= 0.05f; 
        break;
    case 'd':
    case 'D':
        faceX += 0.05f; 
        break;
        
    // Scaling controls
    case '+':
    case '=':
        faceScale += 0.1f;
        if (faceScale > 2.0f) faceScale = 2.0f;
        break;
    case '-':
    case '_':
        faceScale -= 0.1f; 
        if (faceScale < 0.2f) faceScale = 0.2f; 
        break;
        
    // Smile/Frown toggle
    case 'm':
    case 'M':
        isSmiling = !isSmiling;
        updateMouthVBO(); // Update mouth VBO with new smile state
        break;
        
    // Color controls
    case 'b':
    case 'B':
        randomColor(backgroundColor); 
        break;
        
    // Reset function
    case 'r':
    case 'R':
        // Reset all face properties
        faceX = 0.0f;             
        faceY = 0.0f;              
        faceRotation = 0.0f;      
        faceScale = 1.0f;           
        randomColor(leftEyeColor);  
        randomColor(rightEyeColor); 
        backgroundColor[0] = 0.2f; 
        backgroundColor[1] = 0.5f;
        backgroundColor[2] = 0.9f;
        isSmiling = true;
        updateMouthVBO(); // Update mouth VBO after reset
        break;
    }
    
    glutPostRedisplay();
}

// Mouse Controls
void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            // Left click changes left eye color
            randomColor(leftEyeColor);
            std::cout << "Left eye color changed.\n";
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            // Right click changes right eye color
            randomColor(rightEyeColor);
            std::cout << "Right eye color changed.\n";
        }
        
        glutPostRedisplay();
    }
}

// Initialize OpenGL
void init() {
    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(err));
        exit(1);
    }
    
    // Check if VBO is supported
    if (!GLEW_ARB_vertex_buffer_object) {
        fprintf(stderr, "VBO not supported\n");
        exit(1);
    }
    
    // Set initial background color using our stored RGB values
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
    
    // Set up the projection matrix 
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();        
    
    // Create 2D orthographic projection
    gluOrtho2D(-1, 1, -1, 1);
    
    // Set up the modelview matrix
    glMatrixMode(GL_MODELVIEW);   
    glLoadIdentity();           
    
    // Enable anti-aliasing 
    glEnable(GL_LINE_SMOOTH);                  
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);   
    
    // Initialize random number generator with current time as seed
    srand(time(0));
    
    // Generate initial random colors for both eyes
    randomColor(leftEyeColor);
    randomColor(rightEyeColor);
    
    // Initialize VBOs
    initializeVBOs();
}

// Cleanup VBOs
void cleanup() {
    // Delete VBOs
    glDeleteBuffers(1, &headVBO);
    glDeleteBuffers(1, &leftEyeVBO);
    glDeleteBuffers(1, &rightEyeVBO);
    glDeleteBuffers(1, &mouthVBO);
    
    // Delete VAOs
    glDeleteVertexArrays(1, &headVAO);
    glDeleteVertexArrays(1, &leftEyeVAO);
    glDeleteVertexArrays(1, &rightEyeVAO);
    glDeleteVertexArrays(1, &mouthVAO);
}

// Control Instructions
void printInstructions() {
    cout << "==============================\n";
    cout << "   OPENGL FACE CONTROLS (VBO) \n";
    cout << "==============================\n";
    cout << "[ESC]         - Exit Program\n";
    cout << "[W]           - Move Face Up\n";
    cout << "[S]           - Move Face Down\n";
    cout << "[A]           - Move Face Left\n";
    cout << "[D]           - Move Face Right\n";
    cout << "[+/=]         - Scale Face Up\n";
    cout << "[-]           - Scale Face Down\n";
    cout << "[M]           - Toggle Smile/Frown\n";
    cout << "[B]           - Random Background Color\n";
    cout << "[R]           - Reset Face to Default\n";
    cout << "[Left Click]  - Change Left Eye Color\n";
    cout << "[Right Click] - Change Right Eye Color\n";
    cout << "==============================\n";
    cout << "Using Vertex Buffer Objects for improved performance!\n";
    cout << "==============================\n";
}

int main(int argc, char** argv) {
    // Initialize GLUT library
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Interactive OpenGL Face");

    // Show the control instructions to the user
    printInstructions();
    init();

    glutDisplayFunc(display);  
    glutKeyboardFunc(keyboard); 
    glutMouseFunc(mouse);       
    glutTimerFunc(16, timer, 0); 
    glutMainLoop();
    
    return 0;  
}
