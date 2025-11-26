
// Diaz_MarkEron_Icosahedron.cpp
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

// Icosahedron vertex data
#define X 0.525731112119133606f
#define Z 0.850650808352039932f

// Vertices of a unit icosahedron
static GLfloat vdata[12][3] = {
	{-X, 0.0f, Z}, {X, 0.0f, Z}, {-X, 0.0f, -Z}, {X, 0.0f, -Z},
	{0.0f, Z, X}, {0.0f, Z, -X}, {0.0f, -Z, X}, {0.0f, -Z, -X},
	{Z, X, 0.0f}, {-Z, X, 0.0f}, {Z, -X, 0.0f}, {-Z, -X, 0.0f}
};

// Triangle indices for the 20 faces
static GLuint tindices[20][3] = {
	{0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
	{8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
	{7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
	{6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
};

// Camera
float camAngleY = 20.0f;
float camAngleX = 30.0f;
float camDistance = 4.0f;

// Rotation
float rotX = 0.0f, rotY = 0.0f;
float rotSpeed = 0.0f; 
int rotDirection = 1; 

// Timing
int lastTime = 0;

// Window
int winWidth = 800, winHeight = 600;

// Material base color 
GLfloat baseColor[3] = {0.1f, 0.5f, 0.9f}; 

// Ambient light
GLfloat ambientModel[4] = {0.05f, 0.05f, 0.05f, 1.0f};

// Function declarations
void initGL();
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void idle();
void drawHUD();
void drawIcosahedronMesh();

// Initialize OpenGL settings and lighting
void initGL()
{
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	// Lighting
	glEnable(GL_LIGHTING);

	// Soft directional light from above to create top-to-bottom gradient
	glEnable(GL_LIGHT0);
	GLfloat light0_diff[] = {0.6f, 0.6f, 0.7f, 1.0f};
	GLfloat light0_dir[] = {0.0f, -1.0f, 0.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_dir);
	GLfloat light0_amb[] = {0.1f, 0.1f, 0.12f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_amb);

	// Global ambient
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientModel);

	// Enable color material so glColor sets material ambient+diffuse
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// Configure three colored spotlights
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);

	// Spotlight 1 
	GLfloat light1_pos[] = {2.0f, 2.0f, 2.0f, 1.0f};
	GLfloat light1_dir[] = {-2.0f, -2.0f, -2.0f};
	GLfloat light1_diff[] = {0.9f, 0.2f, 0.2f, 1.0f};
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_diff);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_dir);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0f);

	// Spotlight 2 
	GLfloat light2_pos[] = {-2.0f, 2.0f, 2.0f, 1.0f};
	GLfloat light2_dir[] = {2.0f, -2.0f, -2.0f};
	GLfloat light2_diff[] = {0.2f, 0.8f, 0.2f, 1.0f};
	glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diff);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_diff);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0f);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_dir);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 20.0f);

	// Spotlight 3 
	GLfloat light3_pos[] = {0.0f, -2.0f, 3.0f, 1.0f};
	GLfloat light3_dir[] = {0.0f, 2.0f, -3.0f};
	GLfloat light3_diff[] = {0.2f, 0.3f, 0.9f, 1.0f};
	glLightfv(GL_LIGHT3, GL_POSITION, light3_pos);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diff);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light3_diff);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 35.0f);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_dir);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 10.0f);

	// Material specular and shininess
	GLfloat mat_spec[] = {0.9f, 0.9f, 0.9f, 1.0f};
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
	glMaterialf(GL_FRONT, GL_SHININESS, 64.0f);

	// Background color
	glClearColor(0.05f, 0.05f, 0.08f, 1.0f);

	lastTime = glutGet(GLUT_ELAPSED_TIME);
}

// Draw the icosahedron mesh
void drawIcosahedronMesh()
{
	// Use GLUT's built-in icosahedron to satisfy the requirement.
	// We keep lighting/material enabled so the lighting setup creates a gradient-like shading.
	glPushMatrix();
	// scale slightly for visibility
	glScalef(1.0f, 1.0f, 1.0f);
	// Set base material color
	glColor3fv(baseColor);
	glutSolidIcosahedron();
	glPopMatrix();
}

// Display callback
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 3D camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)winWidth / (double)winHeight, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Compute camera position from spherical coords
	float camX = camDistance * sinf(camAngleX * M_PI / 180.0f) * cosf(camAngleY * M_PI / 180.0f);
	float camZ = camDistance * cosf(camAngleX * M_PI / 180.0f) * cosf(camAngleY * M_PI / 180.0f);
	float camY = camDistance * sinf(camAngleY * M_PI / 180.0f);

	gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// Apply rotation animation
	glPushMatrix();
	glRotatef(rotX, 1.0f, 0.0f, 0.0f);
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);

	// Draw the icosahedron mesh
	drawIcosahedronMesh();

	glPopMatrix();

	// Draw HUD overlay
	drawHUD();

	glutSwapBuffers();
}

// Reshape callback
void reshape(int w, int h)
{
	winWidth = w;
	winHeight = (h == 0) ? 1 : h;
	glViewport(0, 0, winWidth, winHeight);
}

// Draw text string at given 2D position
void drawString(float x, float y, const char *s)
{
	glRasterPos2f(x, y);
	for (const char *c = s; *c != '\0'; ++c) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}
}

// Draw text at 2D position with selectable GLUT bitmap font
void drawText(float x, float y, const char *s, void *font)
{
	glRasterPos2f(x, y);
	for (const char *c = s; *c != '\0'; ++c) glutBitmapCharacter(font, *c);
}

// Draw Heads-Up Display (HUD) with a modern semi-transparent panel
void drawHUD()
{
	// Prepare orthographic projection for HUD
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Save and change GL state for HUD
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const float pad = 12.0f;
	const float boxW = 360.0f;
	const float lineH = 20.0f;
	const int lines = 7;
	const float boxH = lines * lineH + pad * 2.0f;
	const float x = 12.0f;
	const float yTop = (float)winHeight - 12.0f;
	const float y0 = yTop - boxH;

	// Draw semi-transparent background panel
	glColor4f(0.06f, 0.06f, 0.08f, 0.78f);
	glBegin(GL_QUADS);
		glVertex2f(x, yTop);
		glVertex2f(x + boxW, yTop);
		glVertex2f(x + boxW, y0);
		glVertex2f(x, y0);
	glEnd();

	// Accent header bar
	glColor4f(0.10f, 0.55f, 0.95f, 0.95f);
	glBegin(GL_QUADS);
		glVertex2f(x, yTop);
		glVertex2f(x + boxW, yTop);
		glVertex2f(x + boxW, yTop - 28.0f);
		glVertex2f(x, yTop - 28.0f);
	glEnd();

	// Title text
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	drawText(x + 10.0f, yTop - 20.0f, "Icosahedron Controls", GLUT_BITMAP_HELVETICA_18);

	// Control lines
	float ty = yTop - 44.0f;
	void *font = GLUT_BITMAP_HELVETICA_12;
	const char *linesText[] = {
		"Arrow keys: Manual rotation",
		"SPACE: Clockwise  TAB: Counter-clockwise",
		"+ or =: Increase speed   -: Decrease speed",
		"BACKSPACE: Stop rotation   ESC: Exit",
		"Camera: W/S zoom   A/D rotate azimuth   Z/X tilt"
	};

	for (int i = 0; i < 5; ++i) {
		drawText(x + 14.0f, ty - i * lineH, linesText[i], font);
	}

	// Restore GL state
	glPopAttrib();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

// Keyboard callback
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27: // ESC
			exit(0);
			break;
		case ' ': // SPACE - clockwise rotation
			rotDirection = 1;
			if (rotSpeed == 0.0f) rotSpeed = 30.0f;
			break;
		case '\t': // TAB - counter-clockwise
			rotDirection = -1;
			if (rotSpeed == 0.0f) rotSpeed = 30.0f;
			break;
		case '+': // increase speed
			rotSpeed += 10.0f;
			break;
		case '=': // increase speed
			rotSpeed += 10.0f;
			break;
		case '-': // decrease speed
			rotSpeed -= 10.0f;
			if (rotSpeed < 0.0f) rotSpeed = 0.0f;
			break;
		case 8: // BACKSPACE - stop rotation
			rotSpeed = 0.0f;
			break;

		// Camera controls
		case 'w': case 'W': // zoom in
			camDistance -= 0.2f; if (camDistance < 1.0f) camDistance = 1.0f;
			break;
		case 's': case 'S': // zoom out
			camDistance += 0.2f;
			break;
		case 'a': case 'A': // rotate left
			camAngleX -= 5.0f;
			break;
		case 'd': case 'D': // rotate right
			camAngleX += 5.0f;
			break;
		case 'z': case 'Z': // tilt up
			camAngleY -= 5.0f; if (camAngleY < -89.0f) camAngleY = -89.0f;
			break;
		case 'x': case 'X': // tilt down
			camAngleY += 5.0f; if (camAngleY > 89.0f) camAngleY = 89.0f;
			break;
	}
	glutPostRedisplay();
}

// Special keys callback
void special(int key, int x, int y)
{
	const float step = 5.0f;
	switch (key) {
		case GLUT_KEY_LEFT:
			rotY -= step; break;
		case GLUT_KEY_RIGHT:
			rotY += step; break;
		case GLUT_KEY_UP:
			rotX -= step; break;
		case GLUT_KEY_DOWN:
			rotX += step; break;
	}
	glutPostRedisplay();
}

// Idle callback for animation
void idle()
{
	int now = glutGet(GLUT_ELAPSED_TIME);
	float dt = (now - lastTime) / 1000.0f;
	lastTime = now;

	if (rotSpeed != 0.0f) {
		float delta = rotDirection * rotSpeed * dt;
		rotY += delta;
		if (rotY > 360.0f) rotY -= 360.0f;
		if (rotY < -360.0f) rotY += 360.0f;
	}

	glutPostRedisplay();
}

// Main function
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Icosahedron - Diaz MarkEron");

	initGL();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
