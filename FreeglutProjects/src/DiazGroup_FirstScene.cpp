// FreeGLUT + GLEW + OpenAL Audio
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include "AudioManager.h"

const float TWO_PI = 2.0f * 3.1415926f;

// Scene enumeration
enum Scene {
    SCENE_1_BIKE_IN_GARAGE,
    SCENE_2_GARAGE_TAKEOFF,
    SCENE_3_MEETING_FRIENDS,
    SCENE_4_GROUP_BIKING,
    SCENE_5_FINAL_POSE
};

Scene currentScene = SCENE_1_BIKE_IN_GARAGE;
float sceneTimer = 0.0f;
float sceneStartTime = 0.0f;

// Scene 1 & 2 variables
float stickmanX = -3.5f;
float walkSpeed = 0.015f;
float walkPhase = 0.0f;
bool walkingStarted = false;
bool isMounted = false;
float bikeX = 1.8f;
float mountProgress = 0.0f;

// Scene 3 variables - friends with bikes
struct Friend {
    float x, z;
    float idlePhase;
    float bikeRotation;
};
Friend friends[4] = {
    {-1.0f, -2.0f, 0.0f, 0.0f},
    {0.5f, -2.5f, 0.5f, 0.0f},
    {2.0f, -2.0f, 1.0f, 0.0f},
    {3.5f, -2.5f, 1.5f, 0.0f}
};

// Scene 4 variables - group biking
float groupBikeProgress = 0.0f;
float groupBikeSpeed = 0.02f;

// Camera control variables
float cameraDistance = 8.0f;
float cameraAngleY = 0.0f;
float cameraAngleX = 20.0f;
float cameraTargetX = 0.0f;
float cameraTargetY = 1.0f;
float cameraTargetZ = 0.0f;
bool manualCameraControl = true; // Only enabled in scenes 1 and 3

// Audio manager
AudioManager* audioManager = nullptr;
int backgroundMusicIndex = -1;
int footstepSoundIndex = -1;
bool musicInitialized = false;

// Enhanced lighting setup
GLfloat lightPos[] = { 3.0f, 4.0f, 2.0f, 1.0f };
GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat ambientLight[] = { 0.3f, 0.3f, 0.4f, 1.0f };
GLfloat specularLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };

// Material properties
GLfloat matShininess = 32.0f;

// Draw a cylinder along +Z from z=0..height (gluCylinder)
void drawCylinder(float base, float top, float height, int slices) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluCylinder(quad, base, top, height, slices, 1);
    gluDeleteQuadric(quad);
}

void drawSphereSolid(float radius) {
    glutSolidSphere(radius, 24, 24);
}

// Draw colorful stickman with proper walking animation
void drawStickman(float phase) {
    // Body proportions
    float headRadius = 0.08f;
    float headY = 1.5f;
    float shoulderY = 1.3f;
    float hipY = 0.9f;
    float footY = 0.0f;
    
    // Walking animation parameters
    float legSwing = sinf(phase) * 0.25f; // Leg swing amplitude
    float armSwing = sinf(phase + 3.14159f) * 0.15f; // Arms opposite to legs
    float bodyBob = fabsf(sinf(phase * 2.0f)) * 0.03f; 
    
    // Adjust all Y positions for body bobbing
    headY += bodyBob;
    shoulderY += bodyBob;
    hipY += bodyBob;
    
    // ---------- Head ----------
    glDisable(GL_LIGHTING);
    
    // Simple circle head for stickman
    glColor3f(0.85f, 0.7f, 0.6f); // skin color
    glPointSize(16.0f);
    glBegin(GL_POINTS);
    glVertex3f(0.0f, headY, 0.0f);
    glEnd();
    
    // Safety helmet (half-circle covering top of head)
    glColor3f(0.9f, 0.4f, 0.7f); // bright pink helmet
    glLineWidth(4.0f);
    
    // Draw helmet as half-circle arc covering top of head
    float helmetRadius = 0.1f; 
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 12; i++) {
        float angle = i * 3.14159f / 12.0f; 
        float x = cosf(angle) * helmetRadius;
        float y = sinf(angle) * helmetRadius;
        glVertex3f(x, headY + y, 0.0f);
    }
    glEnd();
    
    // Helmet visor
    glColor3f(0.7f, 0.3f, 0.5f); // darker pink visor
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex3f(-0.06f, headY - 0.02f, 0.03f);
    glVertex3f(0.06f, headY - 0.02f, 0.03f);
    glEnd();
    
    // ---------- Body (torso) ----------
    glColor3f(0.4f, 0.4f, 0.45f); // gray work shirt
    glLineWidth(8.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, shoulderY, 0.0f);
    glVertex3f(0.0f, hipY, 0.0f);
    glEnd();
    
    // ---------- Arms ----------
    glColor3f(0.85f, 0.7f, 0.6f); // skin color
    glLineWidth(6.0f);
    
    // Calculate arm positions
    float rightShoulderX = 0.0f;
    float rightElbowX = rightShoulderX + 0.2f + armSwing;
    float rightElbowY = shoulderY - 0.2f;
    
    float leftShoulderX = 0.0f;
    float leftElbowX = leftShoulderX - 0.2f - armSwing;
    float leftElbowY = shoulderY - 0.2f;
    
    // Right arm
    glBegin(GL_LINES);
    glVertex3f(rightShoulderX, shoulderY, 0.0f);
    glVertex3f(rightElbowX, rightElbowY, 0.0f);
    glEnd();
    
    // Right forearm
    float rightHandX = rightElbowX + 0.15f;
    float rightHandY = rightElbowY - 0.15f;
    glBegin(GL_LINES);
    glVertex3f(rightElbowX, rightElbowY, 0.0f);
    glVertex3f(rightHandX, rightHandY, 0.0f);
    glEnd();
    
    // Left arm
    glBegin(GL_LINES);
    glVertex3f(leftShoulderX, shoulderY, 0.0f);
    glVertex3f(leftElbowX, leftElbowY, 0.0f);
    glEnd();
    
    // Left forearm
    float leftHandX = leftElbowX - 0.15f;
    float leftHandY = leftElbowY - 0.15f;
    glBegin(GL_LINES);
    glVertex3f(leftElbowX, leftElbowY, 0.0f);
    glVertex3f(leftHandX, leftHandY, 0.0f);
    glEnd();
    
    // ---------- Legs ----------
    glColor3f(0.2f, 0.25f, 0.3f); // dark blue work pants
    glLineWidth(8.0f);
    
    // Calculate leg positions with proper walking motion
    float rightLegPhase = sinf(phase);
    float leftLegPhase = sinf(phase + 3.14159f); // Left leg opposite to right
    
    float rightHipX = 0.05f;
    float rightKneeX = rightHipX + rightLegPhase * 0.15f;
    float rightKneeY = hipY - 0.35f + fabsf(rightLegPhase) * 0.05f; // Knee lifts during swing
    
    float leftHipX = -0.05f;
    float leftKneeX = leftHipX + leftLegPhase * 0.15f;
    float leftKneeY = hipY - 0.35f + fabsf(leftLegPhase) * 0.05f;
    
    // Right leg
    glBegin(GL_LINES);
    glVertex3f(rightHipX, hipY, 0.0f);
    glVertex3f(rightKneeX, rightKneeY, 0.0f);
    glEnd();
    
    float rightFootX = rightKneeX + rightLegPhase * 0.2f;
    glBegin(GL_LINES);
    glVertex3f(rightKneeX, rightKneeY, 0.0f);
    glVertex3f(rightFootX, footY, 0.0f);
    glEnd();
   
    // Left leg
    glBegin(GL_LINES);
    glVertex3f(leftHipX, hipY, 0.0f);
    glVertex3f(leftKneeX, leftKneeY, 0.0f);
    glEnd();
    
    float leftFootX = leftKneeX + leftLegPhase * 0.2f;
    glBegin(GL_LINES);
    glVertex3f(leftKneeX, leftKneeY, 0.0f);
    glVertex3f(leftFootX, footY, 0.0f);
    glEnd();
    
    // ---------- Feet ----------
    glColor3f(0.15f, 0.1f, 0.05f); // dark brown work boots
    glLineWidth(10.0f);
    
    // Right foot
    glBegin(GL_LINES);
    glVertex3f(rightFootX, footY, 0.0f);
    glVertex3f(rightFootX + 0.12f, footY, 0.0f);
    glEnd();
    
    // Left foot
    glBegin(GL_LINES);
    glVertex3f(leftFootX, footY, 0.0f);
    glVertex3f(leftFootX + 0.12f, footY, 0.0f);
    glEnd();
    
    // ---------- Subtle joint dots ----------
    glColor3f(0.6f, 0.6f, 0.6f); // muted gray joints
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    // Shoulders
    glVertex3f(0.0f, shoulderY, 0.0f);
    // Elbows
    glVertex3f(rightElbowX, rightElbowY, 0.0f);
    glVertex3f(leftElbowX, leftElbowY, 0.0f);
    // Hands
    glVertex3f(rightHandX, rightHandY, 0.0f);
    glVertex3f(leftHandX, leftHandY, 0.0f);
    // Hips
    glVertex3f(rightHipX, hipY, 0.0f);
    glVertex3f(leftHipX, hipY, 0.0f);
    // Knees
    glVertex3f(rightKneeX, rightKneeY, 0.0f);
    glVertex3f(leftKneeX, leftKneeY, 0.0f);
    // Feet
    glVertex3f(rightFootX, footY, 0.0f);
    glVertex3f(leftFootX, footY, 0.0f);
    glEnd();
    
    glEnable(GL_LIGHTING);
}

// Road bike with proper geometry and details
void drawBike() {
    glPushMatrix();
    glDisable(GL_LIGHTING);
    
    // Wheels - thin road bike wheels
    glColor3f(0.1f, 0.1f, 0.1f); // black tires
    glLineWidth(8.0f);
    
    // Front wheel
    glPushMatrix();
    glTranslatef(-0.6f, 0.25f, 0.0f);
    // Draw wheel as circle
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 32; i++) {
        float angle = i * 2.0f * 3.14159f / 32.0f;
        glVertex3f(cosf(angle) * 0.25f, sinf(angle) * 0.25f, 0.0f);
    }
    glEnd();
    glPopMatrix();
    
    // Rear wheel
    glPushMatrix();
    glTranslatef(0.6f, 0.25f, 0.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 32; i++) {
        float angle = i * 2.0f * 3.14159f / 32.0f;
        glVertex3f(cosf(angle) * 0.25f, sinf(angle) * 0.25f, 0.0f);
    }
    glEnd();
    glPopMatrix();
    
    // Wheel spokes
    glColor3f(0.7f, 0.7f, 0.8f); // silver spokes
    glLineWidth(2.0f);
    
    // Front wheel spokes
    glBegin(GL_LINES);
    for (int i = 0; i < 8; i++) {
        float angle = i * 2.0f * 3.14159f / 8.0f;
        glVertex3f(-0.6f, 0.25f, 0.0f);
        glVertex3f(-0.6f + cosf(angle) * 0.25f, 0.25f + sinf(angle) * 0.25f, 0.0f);
    }
    glEnd();
    
    // Rear wheel spokes
    glBegin(GL_LINES);
    for (int i = 0; i < 8; i++) {
        float angle = i * 2.0f * 3.14159f / 8.0f;
        glVertex3f(0.6f, 0.25f, 0.0f);
        glVertex3f(0.6f + cosf(angle) * 0.25f, 0.25f + sinf(angle) * 0.25f, 0.0f);
    }
    glEnd();
    
    // Frame - road bike geometry
    glColor3f(0.9f, 0.4f, 0.7f); // pink frame
    glLineWidth(5.0f);
    
    // Main triangle
    glBegin(GL_LINES);
    // Top tube
    glVertex3f(-0.3f, 0.65f, 0.0f); glVertex3f(0.3f, 0.5f, 0.0f);
    // Down tube
    glVertex3f(-0.3f, 0.65f, 0.0f); glVertex3f(0.1f, 0.35f, 0.0f);
    // Seat tube
    glVertex3f(0.1f, 0.35f, 0.0f); glVertex3f(0.3f, 0.5f, 0.0f);
    // Chain stays
    glVertex3f(0.1f, 0.35f, 0.0f); glVertex3f(0.6f, 0.25f, 0.0f);
    // Seat stays
    glVertex3f(0.3f, 0.5f, 0.0f); glVertex3f(0.6f, 0.25f, 0.0f);
    glEnd();
    
    // Fork
    glBegin(GL_LINES);
    glVertex3f(-0.3f, 0.65f, 0.0f); glVertex3f(-0.6f, 0.25f, 0.0f);
    glEnd();
    
    // Handlebars - drop bars for road bike
    glColor3f(0.3f, 0.3f, 0.3f); // dark handlebar tape
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    // Main bar
    glVertex3f(-0.4f, 0.7f, -0.1f); glVertex3f(-0.4f, 0.7f, 0.1f);
    // Drop sections
    glVertex3f(-0.4f, 0.7f, -0.1f); glVertex3f(-0.45f, 0.6f, -0.08f);
    glVertex3f(-0.4f, 0.7f, 0.1f); glVertex3f(-0.45f, 0.6f, 0.08f);
    // Stem
    glVertex3f(-0.35f, 0.67f, 0.0f); glVertex3f(-0.4f, 0.7f, 0.0f);
    glEnd();
    
    // Seat
    glColor3f(0.1f, 0.1f, 0.1f); // black seat
    glLineWidth(8.0f);
    glBegin(GL_LINES);
    glVertex3f(0.25f, 0.7f, -0.05f); glVertex3f(0.35f, 0.7f, 0.05f);
    glEnd();
    
    // Seat post
    glColor3f(0.7f, 0.7f, 0.8f); // silver post
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex3f(0.3f, 0.5f, 0.0f); glVertex3f(0.3f, 0.7f, 0.0f);
    glEnd();
    
    // Pedals and crank
    glColor3f(0.3f, 0.3f, 0.3f); // black pedals
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    // Crank arms
    glVertex3f(0.1f, 0.35f, 0.0f); glVertex3f(0.05f, 0.25f, 0.0f);
    glVertex3f(0.1f, 0.35f, 0.0f); glVertex3f(0.15f, 0.45f, 0.0f);
    // Pedals
    glVertex3f(0.03f, 0.25f, 0.0f); glVertex3f(0.07f, 0.25f, 0.0f);
    glVertex3f(0.13f, 0.45f, 0.0f); glVertex3f(0.17f, 0.45f, 0.0f);
    glEnd();
    
    // Chain
    glColor3f(0.5f, 0.5f, 0.5f); // metallic chain
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // Chain line (simplified)
    glVertex3f(0.1f, 0.35f, 0.0f); glVertex3f(0.6f, 0.25f, 0.0f);
    glEnd();
    
    // Brake cables
    glColor3f(0.6f, 0.6f, 0.6f); // silver cables
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glVertex3f(-0.4f, 0.7f, 0.0f); glVertex3f(-0.6f, 0.4f, 0.0f); // front brake
    glVertex3f(-0.35f, 0.67f, 0.0f); glVertex3f(0.6f, 0.4f, 0.0f); // rear brake
    glEnd();
    
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

// Modern aesthetic garage with enhanced materials and details
void drawGarage() {
    glEnable(GL_LIGHTING);
    
    // Modern tiled floor with slight reflectivity
    GLfloat floorDiffuse[] = { 0.85f, 0.85f, 0.9f, 1.0f };
    GLfloat floorSpecular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, floorDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, floorSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 64.0f);
    
    // Create a tiled floor pattern
    for (int x = -5; x < 5; x++) {
        for (int z = -5; z < 5; z++) {
            // Alternate tile colors slightly
            if ((x + z) % 2 == 0) {
                GLfloat tile1[] = { 0.9f, 0.9f, 0.95f, 1.0f };
                glMaterialfv(GL_FRONT, GL_DIFFUSE, tile1);
            } else {
                GLfloat tile2[] = { 0.8f, 0.8f, 0.85f, 1.0f };
                glMaterialfv(GL_FRONT, GL_DIFFUSE, tile2);
            }
            
            glBegin(GL_QUADS);
            glNormal3f(0, 1, 0);
            glVertex3f(x, 0, z);
            glVertex3f(x+1, 0, z);
            glVertex3f(x+1, 0, z+1);
            glVertex3f(x, 0, z+1);
            glEnd();
        }
    }

    // Modern back wall with gradient-like effect
    GLfloat wallDiffuse[] = { 0.4f, 0.45f, 0.5f, 1.0f };
    GLfloat wallSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wallDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wallSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);
    
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-5, 0, -5);
    glVertex3f(5, 0, -5);
    glVertex3f(5, 3, -5);
    glVertex3f(-5, 3, -5);
    glEnd();

    // Modern ceiling with recessed lighting effect
    GLfloat ceilingDiffuse[] = { 0.95f, 0.95f, 0.95f, 1.0f };
    GLfloat ceilingSpecular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ceilingDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ceilingSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 32.0f);
    
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
    glVertex3f(-5, 3, -5);
    glVertex3f(5, 3, -5);
    glVertex3f(5, 3, 5);
    glVertex3f(-5, 3, 5);
    glEnd();
    
    // Add some ceiling light fixtures
    GLfloat lightFixtureDiffuse[] = { 0.2f, 0.2f, 0.25f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lightFixtureDiffuse);
    
    glPushMatrix();
    glTranslatef(-2.0f, 2.8f, 0.0f);
    glScalef(0.8f, 0.1f, 0.8f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(2.0f, 2.8f, 0.0f);
    glScalef(0.8f, 0.1f, 0.8f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void setupCamera() {
    float cameraX, cameraY, cameraZ;
    
    switch (currentScene) {
        case SCENE_1_BIKE_IN_GARAGE:
            // Manual camera control
            if (manualCameraControl) {
                cameraX = cameraTargetX + cameraDistance * sinf(cameraAngleY * 3.14159f / 180.0f) * cosf(cameraAngleX * 3.14159f / 180.0f);
                cameraZ = cameraTargetZ + cameraDistance * cosf(cameraAngleY * 3.14159f / 180.0f) * cosf(cameraAngleX * 3.14159f / 180.0f);
                cameraY = cameraTargetY + cameraDistance * sinf(cameraAngleX * 3.14159f / 180.0f);
                gluLookAt(cameraX, cameraY, cameraZ,
                         cameraTargetX, cameraTargetY, cameraTargetZ,
                         0.0, 1.0, 0.0);
            }
            break;
            
        case SCENE_2_GARAGE_TAKEOFF:
            // Automatic smooth follow camera
            cameraX = stickmanX + 3.0f;
            cameraY = 2.5f;
            cameraZ = 5.0f;
            gluLookAt(cameraX, cameraY, cameraZ,
                     stickmanX, 1.0f, 0.0f,
                     0.0, 1.0, 0.0);
            break;
            
        case SCENE_3_MEETING_FRIENDS:
            // Manual camera control
            if (manualCameraControl) {
                cameraX = cameraTargetX + cameraDistance * sinf(cameraAngleY * 3.14159f / 180.0f) * cosf(cameraAngleX * 3.14159f / 180.0f);
                cameraZ = cameraTargetZ + cameraDistance * cosf(cameraAngleY * 3.14159f / 180.0f) * cosf(cameraAngleX * 3.14159f / 180.0f);
                cameraY = cameraTargetY + cameraDistance * sinf(cameraAngleX * 3.14159f / 180.0f);
                gluLookAt(cameraX, cameraY, cameraZ,
                         cameraTargetX, cameraTargetY, cameraTargetZ,
                         0.0, 1.0, 0.0);
            }
            break;
            
        case SCENE_4_GROUP_BIKING:
            // Automatic wide side-follow camera
            cameraX = groupBikeProgress * 0.5f - 2.0f;
            cameraY = 3.0f;
            cameraZ = 8.0f;
            gluLookAt(cameraX, cameraY, cameraZ,
                     groupBikeProgress + 1.0f, 1.5f, 0.0f,
                     0.0, 1.0, 0.0);
            break;
            
        case SCENE_5_FINAL_POSE: {
            // Automatic slow zoom-in
            float zoomProgress = fminf(sceneTimer / 5.0f, 1.0f);
            float zoomDist = 12.0f - zoomProgress * 5.0f;
            cameraX = 2.0f + zoomDist * 0.3f;
            cameraY = 2.0f + zoomProgress * 0.5f;
            cameraZ = zoomDist;
            gluLookAt(cameraX, cameraY, cameraZ,
                     2.0f, 1.5f, 0.0f,
                     0.0, 1.0, 0.0);
            break;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    setupCamera();

    // Enhanced lighting setup
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Main light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    
    // Ambient lighting
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    
    // Add a second light for fill lighting
    glEnable(GL_LIGHT1);
    GLfloat light1Pos[] = { -2.0f, 3.0f, 4.0f, 1.0f };
    GLfloat light1Color[] = { 0.6f, 0.7f, 0.8f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Color);

    // Draw scene based on current scene
    switch (currentScene) {
        case SCENE_1_BIKE_IN_GARAGE:
            drawGarage();
            // Bike in garage
            glPushMatrix();
            glTranslatef(bikeX, 0.0f, 0.0f);
            drawBike();
            glPopMatrix();
            // Idle stickman
            glPushMatrix();
            glTranslatef(stickmanX, 0.0f, 0.0f);
            drawStickman(sinf(sceneTimer * 2.0f) * 0.05f); // Subtle idle
            glPopMatrix();
            break;
            
        case SCENE_2_GARAGE_TAKEOFF:
            drawGarage();
            // Stickman mounting and riding bike
            if (isMounted) {
                glPushMatrix();
                glTranslatef(stickmanX, 0.0f, 0.0f);
                drawBike();
                glTranslatef(0.3f, 0.7f, 0.0f); // Stickman on bike
                glScalef(0.8f, 0.8f, 0.8f);
                drawStickman(walkPhase);
                glPopMatrix();
            } else {
                // Moving towards bike
                glPushMatrix();
                glTranslatef(stickmanX, 0.0f, 0.0f);
                drawStickman(walkPhase);
                glPopMatrix();
                // Static bike
                glPushMatrix();
                glTranslatef(bikeX, 0.0f, 0.0f);
                drawBike();
                glPopMatrix();
            }
            break;
            
        case SCENE_3_MEETING_FRIENDS:
            // Draw ground
            drawGarage();
            
            // Draw main character with bike
            glPushMatrix();
            glTranslatef(stickmanX, 0.0f, 0.0f);
            drawBike();
            glTranslatef(0.3f, 0.7f, 0.0f);
            glScalef(0.8f, 0.8f, 0.8f);
            drawStickman(sinf(sceneTimer * 1.5f) * 0.05f);
            glPopMatrix();
            
            // Draw friends with bikes
            for (int i = 0; i < 4; i++) {
                glPushMatrix();
                glTranslatef(friends[i].x, 0.0f, friends[i].z);
                glRotatef(friends[i].bikeRotation, 0.0f, 1.0f, 0.0f);
                drawBike();
                glTranslatef(0.3f, 0.7f, 0.0f);
                glScalef(0.8f, 0.8f, 0.8f);
                drawStickman(sinf(sceneTimer * 1.5f + friends[i].idlePhase) * 0.05f);
                glPopMatrix();
            }
            break;
            
        case SCENE_4_GROUP_BIKING:
            drawGarage();
            
            // Draw all bikers in sync
            for (int i = 0; i < 5; i++) {
                glPushMatrix();
                float xOffset = groupBikeProgress + i * 0.8f - 1.5f;
                float zOffset = (i - 2) * 0.5f;
                glTranslatef(xOffset, 0.0f, zOffset);
                drawBike();
                glTranslatef(0.3f, 0.7f, 0.0f);
                glScalef(0.8f, 0.8f, 0.8f);
                drawStickman(walkPhase + i * 0.3f);
                glPopMatrix();
            }
            break;
            
        case SCENE_5_FINAL_POSE:
            drawGarage();
            
            // Group pose - all bikers stopped in formation
            float formations[5][2] = {
                {0.0f, 0.0f},    // Center
                {-1.0f, -0.5f},  // Left front
                {1.0f, -0.5f},   // Right front
                {-1.5f, -1.0f},  // Left back
                {1.5f, -1.0f}    // Right back
            };
            
            for (int i = 0; i < 5; i++) {
                glPushMatrix();
                glTranslatef(formations[i][0] + 2.0f, 0.0f, formations[i][1]);
                drawBike();
                glTranslatef(0.3f, 0.7f, 0.0f);
                glScalef(0.8f, 0.8f, 0.8f);
                // Victory pose - arms raised
                drawStickman(0.0f);
                glPopMatrix();
            }
            break;
    }

    glutSwapBuffers();
}

void updateScene1() {
    // Just idle - wait for user to advance
}

void updateScene2() {
    if (!walkingStarted) {
        // Wait a moment before starting
        if (sceneTimer >= 1.0f) {
            walkingStarted = true;
        }
        return;
    }
    
    if (!isMounted) {
        // Walk to bike
        if (stickmanX < bikeX - 0.5f) {
            stickmanX += walkSpeed;
            walkPhase += 0.15f;
        } else {
            // Mount the bike
            isMounted = true;
            stickmanX = bikeX;
        }
    } else {
        // Ride out of garage
        if (stickmanX < 8.0f) {
            stickmanX += walkSpeed * 2.5f; // Faster on bike
            walkPhase += 0.2f;
        } else {
            // Scene complete - auto advance to scene 3
            currentScene = SCENE_3_MEETING_FRIENDS;
            sceneTimer = 0.0f;
            stickmanX = -2.0f;
            cameraDistance = 10.0f;
            cameraAngleY = 0.0f;
            cameraAngleX = 20.0f;
            cameraTargetX = 1.0f;
            manualCameraControl = true;
        }
    }
}

void updateScene3() {
    // Idle animations for friends
    for (int i = 0; i < 4; i++) {
        friends[i].idlePhase += 0.01f;
    }
}

void updateScene4() {
    // Group biking forward
    if (groupBikeProgress < 15.0f) {
        groupBikeProgress += groupBikeSpeed;
        walkPhase += 0.2f;
    } else {
        // Scene complete - auto advance to scene 5
        currentScene = SCENE_5_FINAL_POSE;
        sceneTimer = 0.0f;
    }
}

void updateScene5() {
    // Just timing for zoom animation
}

void update(int v) {
    sceneTimer += 0.016f;
    
    switch (currentScene) {
        case SCENE_1_BIKE_IN_GARAGE:
            updateScene1();
            break;
        case SCENE_2_GARAGE_TAKEOFF:
            updateScene2();
            break;
        case SCENE_3_MEETING_FRIENDS:
            updateScene3();
            break;
        case SCENE_4_GROUP_BIKING:
            updateScene4();
            break;
        case SCENE_5_FINAL_POSE:
            updateScene5();
            break;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case ' ': // Space to advance scenes
            if (currentScene == SCENE_1_BIKE_IN_GARAGE) {
                currentScene = SCENE_2_GARAGE_TAKEOFF;
                sceneTimer = 0.0f;
                walkingStarted = false;
                isMounted = false;
                manualCameraControl = false;
                std::cout << "Scene 2: Garage Take Off" << std::endl;
            } else if (currentScene == SCENE_3_MEETING_FRIENDS) {
                currentScene = SCENE_4_GROUP_BIKING;
                sceneTimer = 0.0f;
                groupBikeProgress = 0.0f;
                manualCameraControl = false;
                std::cout << "Scene 4: Group Biking" << std::endl;
            }
            break;
            
        case 27: // ESC to quit
            if (audioManager) {
                delete audioManager;
            }
            exit(0);
            break;
            
        case '1':
            currentScene = SCENE_1_BIKE_IN_GARAGE;
            sceneTimer = 0.0f;
            stickmanX = -3.5f;
            manualCameraControl = true;
            std::cout << "Scene 1: Bike in Garage" << std::endl;
            break;
            
        case '2':
            currentScene = SCENE_2_GARAGE_TAKEOFF;
            sceneTimer = 0.0f;
            stickmanX = -3.5f;
            walkingStarted = false;
            isMounted = false;
            manualCameraControl = false;
            std::cout << "Scene 2: Garage Take Off" << std::endl;
            break;
            
        case '3':
            currentScene = SCENE_3_MEETING_FRIENDS;
            sceneTimer = 0.0f;
            stickmanX = -2.0f;
            manualCameraControl = true;
            std::cout << "Scene 3: Meeting Friends" << std::endl;
            break;
            
        case '4':
            currentScene = SCENE_4_GROUP_BIKING;
            sceneTimer = 0.0f;
            groupBikeProgress = 0.0f;
            manualCameraControl = false;
            std::cout << "Scene 4: Group Biking" << std::endl;
            break;
            
        case '5':
            currentScene = SCENE_5_FINAL_POSE;
            sceneTimer = 0.0f;
            manualCameraControl = false;
            std::cout << "Scene 5: Final Pose" << std::endl;
            break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    if (!manualCameraControl) return; // Only in scenes 1 and 3
    
    switch (key) {
        case GLUT_KEY_LEFT:
            cameraAngleY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            cameraAngleY += 5.0f;
            break;
        case GLUT_KEY_UP:
            cameraAngleX += 5.0f;
            if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
            break;
        case GLUT_KEY_DOWN:
            cameraAngleX -= 5.0f;
            if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
            break;
    }
    glutPostRedisplay();
}

void mouseWheel(int button, int dir, int x, int y) {
    if (!manualCameraControl) return; // Only in scenes 1 and 3
    
    if (dir > 0) {
        cameraDistance -= 0.5f;
        if (cameraDistance < 2.0f) cameraDistance = 2.0f;
    } else {
        cameraDistance += 0.5f;
        if (cameraDistance > 20.0f) cameraDistance = 20.0f;
    }
    glutPostRedisplay();
}

void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
    
    // Enable better rendering features
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Anti-aliasing for lines and points
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 900.0 / 600.0, 0.1, 100.0); // Slightly narrower FOV for more cinematic look

    glMatrixMode(GL_MODELVIEW);
    // Modern gradient background (dark blue to light gray)
    glClearColor(0.12f, 0.15f, 0.25f, 1.0f);
}

void initAudio() {
    audioManager = new AudioManager();
    
    if (!audioManager->initialize()) {
        std::cerr << "Failed to initialize audio" << std::endl;
        delete audioManager;
        audioManager = nullptr;
        return;
    }
    
    // Load the Kick Buttowski intro music
    backgroundMusicIndex = audioManager->loadSound("../sounds/background.wav");
    
    // Auto-start background music
    if (backgroundMusicIndex >= 0) {
        audioManager->setVolume(backgroundMusicIndex, 0.4f); // Set comfortable volume
        audioManager->playSound(backgroundMusicIndex, true); // Loop background music automatically
    } else {
        std::cerr << "Failed to load background music" << std::endl;
    }
    
    musicInitialized = true;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 600);
    glutCreateWindow("Multi-Scene Bike Animation - Press SPACE to advance | 1-5 for scenes | Arrow keys for camera");

    glewInit();
    initGL();
    initAudio();

    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseWheelFunc(mouseWheel);
    
    std::cout << "=== Controls ===" << std::endl;
    std::cout << "SPACE: Advance to next scene" << std::endl;
    std::cout << "1-5: Jump to specific scene" << std::endl;
    std::cout << "Arrow Keys: Rotate camera (scenes 1 & 3 only)" << std::endl;
    std::cout << "Mouse Wheel: Zoom in/out (scenes 1 & 3 only)" << std::endl;
    std::cout << "ESC: Exit" << std::endl;
    std::cout << "===============" << std::endl;
    std::cout << "Scene 1: Bike in Garage (Manual Camera)" << std::endl;
    
    atexit([]() {
        if (audioManager) {
            delete audioManager;
            audioManager = nullptr;
        }
    });
    
    glutMainLoop();
    return 0;
}