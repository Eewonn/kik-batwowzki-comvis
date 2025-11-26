#include "DrawingUtils.h"

void drawCylinder(float base, float top, float height, int slices) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluCylinder(quad, base, top, height, slices, 1);
    gluDeleteQuadric(quad);
}

void drawSphereSolid(float radius) {
    glutSolidSphere(radius, 24, 24);
}

void drawStickman(float phase, float helmetHue) {
    // Body proportions
    float headRadius = 0.08f;
    float headY = 1.5f;
    float shoulderY = 1.3f;
    float hipY = 0.9f;
    float footY = 0.0f;
    
    // Walking animation parameters
    float legSwing = sinf(phase) * 0.25f;
    float armSwing = sinf(phase + 3.14159f) * 0.15f;
    float bodyBob = fabsf(sinf(phase * 2.0f)) * 0.03f; 
    
    // Adjust all Y positions for body bobbing
    headY += bodyBob;
    shoulderY += bodyBob;
    hipY += bodyBob;
    
    glDisable(GL_LIGHTING);
    
    // Head - warm peach skin
    glColor3f(0.95f, 0.78f, 0.65f);
    glPointSize(18.0f);
    glBegin(GL_POINTS);
    glVertex3f(0.0f, headY, 0.0f);
    glEnd();
    
    // Eyes - cute dots
    glColor3f(0.2f, 0.2f, 0.25f);
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    glVertex3f(-0.025f, headY + 0.02f, 0.05f);
    glVertex3f(0.025f, headY + 0.02f, 0.05f);
    glEnd();
    
    // Helmet with vibrant gradient color
    float helmetR = 0.95f * helmetHue + 0.3f * (1.0f - helmetHue);
    float helmetG = 0.4f + 0.4f * helmetHue;
    float helmetB = 0.85f * (1.0f - helmetHue * 0.3f);
    glColor3f(helmetR, helmetG, helmetB);
    glLineWidth(5.0f);
    float helmetRadius = 0.11f; 
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 14; i++) {
        float angle = i * 3.14159f / 14.0f; 
        float x = cosf(angle) * helmetRadius;
        float y = sinf(angle) * helmetRadius;
        glVertex3f(x, headY + y, 0.0f);
    }
    glEnd();
    
    // Helmet visor - glossy dark
    glColor3f(0.15f, 0.15f, 0.2f);
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    glVertex3f(-0.07f, headY - 0.01f, 0.04f);
    glVertex3f(0.07f, headY - 0.01f, 0.04f);
    glEnd();
    
    // Body (torso) - colorful jersey
    glColor3f(0.3f + helmetHue * 0.4f, 0.5f, 0.7f - helmetHue * 0.3f);
    glLineWidth(10.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, shoulderY, 0.0f);
    glVertex3f(0.0f, hipY, 0.0f);
    glEnd();
    
    // Arms - skin tone
    glColor3f(0.95f, 0.78f, 0.65f);
    glLineWidth(6.0f);
    
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
    
    float leftHandX = leftElbowX - 0.15f;
    float leftHandY = leftElbowY - 0.15f;
    glBegin(GL_LINES);
    glVertex3f(leftElbowX, leftElbowY, 0.0f);
    glVertex3f(leftHandX, leftHandY, 0.0f);
    glEnd();
    
    // Legs - dark athletic shorts
    glColor3f(0.15f, 0.2f, 0.28f);
    glLineWidth(9.0f);
    
    float rightLegPhase = sinf(phase);
    float leftLegPhase = sinf(phase + 3.14159f);
    
    float rightHipX = 0.05f;
    float rightKneeX = rightHipX + rightLegPhase * 0.15f;
    float rightKneeY = hipY - 0.35f + fabsf(rightLegPhase) * 0.05f;
    
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
    
    // Feet - bright sneakers
    glColor3f(1.0f, 0.4f, 0.35f);
    glLineWidth(12.0f);
    
    glBegin(GL_LINES);
    glVertex3f(rightFootX, footY, 0.0f);
    glVertex3f(rightFootX + 0.13f, footY, 0.0f);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(leftFootX, footY, 0.0f);
    glVertex3f(leftFootX + 0.13f, footY, 0.0f);
    glEnd();
    
    // Joint dots - metallic accent
    glColor3f(0.8f, 0.85f, 0.9f);
    glPointSize(7.0f);
    glBegin(GL_POINTS);
    glVertex3f(0.0f, shoulderY, 0.0f);
    glVertex3f(rightElbowX, rightElbowY, 0.0f);
    glVertex3f(leftElbowX, leftElbowY, 0.0f);
    glVertex3f(rightKneeX, rightKneeY, 0.0f);
    glVertex3f(leftKneeX, leftKneeY, 0.0f);
    glEnd();
    
    // Hand dots
    glColor3f(0.95f, 0.78f, 0.65f);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glVertex3f(rightHandX, rightHandY, 0.0f);
    glVertex3f(leftHandX, leftHandY, 0.0f);
    glEnd();
    
    glEnable(GL_LIGHTING);
}

void drawBike() {
    glPushMatrix();
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);  // Face forward
    
    glDisable(GL_LIGHTING);
    
    // ===== COLORFUL WHEELS WITH GRADIENT SPOKES =====
    float wheelRadius = 0.22f;
    int numSpokes = 12;
    
    // Front wheel
    glPushMatrix();
    glTranslatef(-0.6f, 0.25f, 0.0f);
    
    // Outer tire - vibrant teal
    glColor3f(0.1f, 0.85f, 0.9f);
    glLineWidth(5.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 32; i++) {
        float angle = i * 6.28318f / 32.0f;
        glVertex3f(cosf(angle) * wheelRadius, sinf(angle) * wheelRadius, 0.0f);
    }
    glEnd();
    
    // Inner rim - coral pink
    glColor3f(1.0f, 0.5f, 0.6f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 24; i++) {
        float angle = i * 6.28318f / 24.0f;
        glVertex3f(cosf(angle) * (wheelRadius - 0.03f), sinf(angle) * (wheelRadius - 0.03f), 0.0f);
    }
    glEnd();
    
    // Rainbow spokes
    glLineWidth(2.0f);
    for (int i = 0; i < numSpokes; i++) {
        float hue = (float)i / numSpokes;
        glColor3f(0.5f + 0.5f * sinf(hue * 6.28f), 
                  0.5f + 0.5f * sinf(hue * 6.28f + 2.09f), 
                  0.5f + 0.5f * sinf(hue * 6.28f + 4.18f));
        float angle = i * 6.28318f / numSpokes;
        glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(cosf(angle) * (wheelRadius - 0.04f), sinf(angle) * (wheelRadius - 0.04f), 0.0f);
        glEnd();
    }
    
    // Hub - golden yellow
    glColor3f(1.0f, 0.85f, 0.3f);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    glPopMatrix();
    
    // Rear wheel (same style)
    glPushMatrix();
    glTranslatef(0.6f, 0.25f, 0.0f);
    
    glColor3f(0.1f, 0.85f, 0.9f);
    glLineWidth(5.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 32; i++) {
        float angle = i * 6.28318f / 32.0f;
        glVertex3f(cosf(angle) * wheelRadius, sinf(angle) * wheelRadius, 0.0f);
    }
    glEnd();
    
    glColor3f(1.0f, 0.5f, 0.6f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 24; i++) {
        float angle = i * 6.28318f / 24.0f;
        glVertex3f(cosf(angle) * (wheelRadius - 0.03f), sinf(angle) * (wheelRadius - 0.03f), 0.0f);
    }
    glEnd();
    
    glLineWidth(2.0f);
    for (int i = 0; i < numSpokes; i++) {
        float hue = (float)i / numSpokes;
        glColor3f(0.5f + 0.5f * sinf(hue * 6.28f), 
                  0.5f + 0.5f * sinf(hue * 6.28f + 2.09f), 
                  0.5f + 0.5f * sinf(hue * 6.28f + 4.18f));
        float angle = i * 6.28318f / numSpokes;
        glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(cosf(angle) * (wheelRadius - 0.04f), sinf(angle) * (wheelRadius - 0.04f), 0.0f);
        glEnd();
    }
    
    glColor3f(1.0f, 0.85f, 0.3f);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    glPopMatrix();
    
    // ===== GRADIENT FRAME - Hot Pink to Electric Purple =====
    glLineWidth(6.0f);
    
    // Top tube (pink)
    glColor3f(1.0f, 0.3f, 0.6f);
    glBegin(GL_LINES);
    glVertex3f(-0.3f, 0.65f, 0.0f);
    glColor3f(0.9f, 0.4f, 0.9f);  // Transition to magenta
    glVertex3f(0.3f, 0.6f, 0.0f);
    glEnd();
    
    // Down tube (purple to cyan)
    glColor3f(0.7f, 0.2f, 0.95f);
    glBegin(GL_LINES);
    glVertex3f(-0.3f, 0.65f, 0.0f);
    glColor3f(0.3f, 0.7f, 0.95f);
    glVertex3f(0.1f, 0.35f, 0.0f);
    glEnd();
    
    // Seat tube (orange to yellow)
    glColor3f(1.0f, 0.5f, 0.15f);
    glBegin(GL_LINES);
    glVertex3f(0.3f, 0.6f, 0.0f);
    glColor3f(1.0f, 0.75f, 0.2f);
    glVertex3f(0.3f, 0.45f, 0.0f);
    glEnd();
    
    // Chain stays (cyan to teal)
    glColor3f(0.2f, 0.9f, 0.95f);
    glBegin(GL_LINES);
    glVertex3f(0.1f, 0.35f, 0.0f);
    glColor3f(0.1f, 0.7f, 0.8f);
    glVertex3f(0.6f, 0.25f, 0.0f);
    glEnd();
    
    // Seat stays (magenta)
    glColor3f(0.95f, 0.35f, 0.75f);
    glBegin(GL_LINES);
    glVertex3f(0.3f, 0.6f, 0.0f);
    glVertex3f(0.6f, 0.25f, 0.0f);
    glEnd();
    
    // Fork (electric blue)
    glColor3f(0.3f, 0.6f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(-0.3f, 0.65f, 0.0f);
    glVertex3f(-0.6f, 0.25f, 0.0f);
    glEnd();
    
    // ===== HANDLEBARS - Neon Green =====
    glColor3f(0.4f, 1.0f, 0.5f);
    glLineWidth(5.0f);
    glBegin(GL_LINES);
    glVertex3f(-0.4f, 0.7f, -0.12f);
    glVertex3f(-0.4f, 0.7f, 0.12f);
    glEnd();
    
    // Handlebar grips - bright coral
    glColor3f(1.0f, 0.4f, 0.4f);
    glPointSize(12.0f);
    glBegin(GL_POINTS);
    glVertex3f(-0.4f, 0.7f, -0.12f);
    glVertex3f(-0.4f, 0.7f, 0.12f);
    glEnd();
    
    // ===== SEAT - Vibrant cushion =====
    glColor3f(0.9f, 0.2f, 0.5f);  // Hot pink
    glLineWidth(12.0f);
    glBegin(GL_LINES);
    glVertex3f(0.22f, 0.7f, 0.0f);
    glVertex3f(0.38f, 0.7f, 0.0f);
    glEnd();
    
    // Seat post - silver
    glColor3f(0.75f, 0.8f, 0.85f);
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    glVertex3f(0.3f, 0.7f, 0.0f);
    glVertex3f(0.3f, 0.6f, 0.0f);
    glEnd();
    
    // ===== PEDALS & CRANK =====
    glColor3f(0.85f, 0.85f, 0.9f);
    glLineWidth(3.0f);
    // Crank arms
    glBegin(GL_LINES);
    glVertex3f(0.1f, 0.35f, 0.0f);
    glVertex3f(0.1f, 0.2f, 0.06f);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0.1f, 0.35f, 0.0f);
    glVertex3f(0.1f, 0.5f, -0.06f);
    glEnd();
    
    // Pedals - bright yellow
    glColor3f(1.0f, 0.9f, 0.2f);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glVertex3f(0.1f, 0.2f, 0.06f);
    glVertex3f(0.1f, 0.5f, -0.06f);
    glEnd();
    
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void drawGarage() {
    glEnable(GL_LIGHTING);
    
    // Modern checkerboard floor with colorful accents
    GLfloat floorSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, floorSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 80.0f);
    
    // Floor only inside garage (z from -5 to 3 to avoid road overlap)
    for (int x = -5; x < 5; x++) {
        for (int z = -5; z < 3; z++) {
            // Stylish checkerboard with color pops
            if ((x + z) % 2 == 0) {
                GLfloat tile1[] = { 0.92f, 0.92f, 0.95f, 1.0f }; // Light
                glMaterialfv(GL_FRONT, GL_DIFFUSE, tile1);
            } else {
                // Occasional color accent tiles
                if ((x * z) % 7 == 0 && x != 0) {
                    float hue = (float)(abs(x + z) % 4) / 4.0f;
                    GLfloat colorTile[] = { 
                        0.5f + 0.4f * sinf(hue * 6.28f),
                        0.6f + 0.3f * sinf(hue * 6.28f + 2.1f),
                        0.5f + 0.4f * sinf(hue * 6.28f + 4.2f),
                        1.0f 
                    };
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorTile);
                } else {
                    GLfloat tile2[] = { 0.25f, 0.28f, 0.32f, 1.0f }; // Dark
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, tile2);
                }
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

    // Bright pink-tinted wall to match garage theme
    GLfloat wallDiffuse[] = { 0.95f, 0.6f, 0.75f, 1.0f };
    GLfloat wallSpecular[] = { 0.25f, 0.2f, 0.22f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wallDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wallSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);
    
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-5, 0, -5);
    glVertex3f(5, 0, -5);
    glVertex3f(5, 3, -5);
    glVertex3f(-5, 3, -5);
    glEnd();
    
    // Neon accent stripes on wall
    GLfloat neon1[] = { 1.0f, 0.2f, 0.55f, 1.0f }; // Hot pink neon
    GLfloat neonEmit1[] = { 0.4f, 0.08f, 0.22f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, neon1);
    glMaterialfv(GL_FRONT, GL_EMISSION, neonEmit1);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-4.5f, 0.5f, -4.98f);
    glVertex3f(-4.3f, 0.5f, -4.98f);
    glVertex3f(-4.3f, 2.8f, -4.98f);
    glVertex3f(-4.5f, 2.8f, -4.98f);
    glEnd();
    
    GLfloat neon2[] = { 0.2f, 0.9f, 1.0f, 1.0f }; // Cyan neon
    GLfloat neonEmit2[] = { 0.08f, 0.35f, 0.4f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, neon2);
    glMaterialfv(GL_FRONT, GL_EMISSION, neonEmit2);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(4.3f, 0.5f, -4.98f);
    glVertex3f(4.5f, 0.5f, -4.98f);
    glVertex3f(4.5f, 2.8f, -4.98f);
    glVertex3f(4.3f, 2.8f, -4.98f);
    glEnd();
    
    // Horizontal neon stripe
    GLfloat neon3[] = { 0.95f, 0.8f, 0.2f, 1.0f }; // Golden yellow
    GLfloat neonEmit3[] = { 0.38f, 0.32f, 0.08f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, neon3);
    glMaterialfv(GL_FRONT, GL_EMISSION, neonEmit3);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-4.0f, 1.8f, -4.98f);
    glVertex3f(4.0f, 1.8f, -4.98f);
    glVertex3f(4.0f, 1.9f, -4.98f);
    glVertex3f(-4.0f, 1.9f, -4.98f);
    glEnd();
    
    // Reset emission
    GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);

    // Sleek modern ceiling
    GLfloat ceilingDiffuse[] = { 0.95f, 0.95f, 0.98f, 1.0f };
    GLfloat ceilingSpecular[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ceilingDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ceilingSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 40.0f);
    
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
    glVertex3f(-5, 3, -5);
    glVertex3f(5, 3, -5);
    glVertex3f(5, 3, 5);
    glVertex3f(-5, 3, 5);
    glEnd();
    
    // Modern LED strip lights
    GLfloat ledStrip[] = { 1.0f, 0.98f, 0.95f, 1.0f };
    GLfloat ledEmit[] = { 0.9f, 0.88f, 0.82f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ledStrip);
    glMaterialfv(GL_FRONT, GL_EMISSION, ledEmit);
    
    // Left LED strip
    glPushMatrix();
    glTranslatef(-2.5f, 2.9f, 0.0f);
    glScalef(0.15f, 0.05f, 3.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Right LED strip
    glPushMatrix();
    glTranslatef(2.5f, 2.9f, 0.0f);
    glScalef(0.15f, 0.05f, 3.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Center circular light
    glPushMatrix();
    glTranslatef(0.0f, 2.85f, 0.0f);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.08f, 0.5f, 12, 24);
    glPopMatrix();
    
    // Reset emission
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
    
    // Tool rack silhouette on wall
    GLfloat rackColor[] = { 0.35f, 0.38f, 0.42f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, rackColor);
    
    glPushMatrix();
    glTranslatef(3.0f, 2.2f, -4.9f);
    glScalef(1.5f, 0.08f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Tool shapes
    glPushMatrix();
    glTranslatef(2.5f, 1.8f, -4.85f);
    glScalef(0.08f, 0.4f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(3.0f, 1.7f, -4.85f);
    glScalef(0.15f, 0.5f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(3.5f, 1.75f, -4.85f);
    glScalef(0.1f, 0.45f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Draw stickman properly seated on bike
void drawStickmanOnBike(float pedalPhase, float helmetHue) {
    glDisable(GL_LIGHTING);
    
    // Bike reference points (after 180° rotation): seat at (-0.3, 0.7), handlebars at (0.4, 0.7), crank at (-0.1, 0.35)
    float seatX = -0.3f;
    float seatY = 0.7f;
    
    // Stickman positioned on seat
    float hipX = seatX;
    float hipY = seatY;
    float shoulderX = hipX + 0.15f;  // Lean forward
    float shoulderY = hipY + 0.3f;
    float headX = shoulderX + 0.05f;
    float headY = shoulderY + 0.2f;
    
    // Pedaling animation
    float rightPedalAngle = pedalPhase;
    float leftPedalAngle = pedalPhase + 3.14159f;
    
    // Head - warm peachy skin
    glColor3f(0.95f, 0.78f, 0.65f);
    glPointSize(16.0f);
    glBegin(GL_POINTS);
    glVertex3f(headX, headY, 0.0f);
    glEnd();
    
    // Eyes
    glColor3f(0.2f, 0.2f, 0.25f);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glVertex3f(headX + 0.02f, headY + 0.02f, 0.04f);
    glVertex3f(headX + 0.05f, headY + 0.02f, 0.04f);
    glEnd();
    
    // Vibrant gradient helmet
    float helmetR = 0.95f * helmetHue + 0.3f * (1.0f - helmetHue);
    float helmetG = 0.4f + 0.4f * helmetHue;
    float helmetB = 0.85f * (1.0f - helmetHue * 0.3f);
    glColor3f(helmetR, helmetG, helmetB);
    glLineWidth(4.5f);
    float helmetRadius = 0.1f;
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 14; i++) {
        float angle = i * 3.14159f / 14.0f;
        float x = cosf(angle) * helmetRadius;
        float y = sinf(angle) * helmetRadius;
        glVertex3f(headX + x, headY + y, 0.0f);
    }
    glEnd();
    
    // Glossy visor
    glColor3f(0.15f, 0.15f, 0.2f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex3f(headX - 0.04f, headY - 0.01f, 0.03f);
    glVertex3f(headX + 0.06f, headY - 0.01f, 0.03f);
    glEnd();
    
    // Body - colorful cycling jersey (matches helmet theme)
    glColor3f(0.3f + helmetHue * 0.4f, 0.5f, 0.7f - helmetHue * 0.3f);
    glLineWidth(9.0f);
    glBegin(GL_LINES);
    glVertex3f(shoulderX, shoulderY, 0.0f);
    glVertex3f(hipX, hipY, 0.0f);
    glEnd();
    
    // Arms - skin reaching to handlebars
    glColor3f(0.95f, 0.78f, 0.65f);
    glLineWidth(6.0f);
    
    float handlebarX = 0.4f;
    float handlebarY = 0.7f;
    
    // Right arm to handlebar
    glBegin(GL_LINES);
    glVertex3f(shoulderX + 0.05f, shoulderY, 0.0f);
    glVertex3f(handlebarX, handlebarY, 0.08f);
    glEnd();
    
    // Left arm to handlebar
    glBegin(GL_LINES);
    glVertex3f(shoulderX - 0.05f, shoulderY, 0.0f);
    glVertex3f(handlebarX, handlebarY, -0.08f);
    glEnd();
    
    // Cycling gloves (bright accent)
    glColor3f(1.0f, 0.4f, 0.35f);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glVertex3f(handlebarX, handlebarY, 0.08f);
    glVertex3f(handlebarX, handlebarY, -0.08f);
    glEnd();
    
    // Legs - dark cycling shorts
    glColor3f(0.15f, 0.2f, 0.28f);
    glLineWidth(7.0f);
    
    // Crank position
    float crankCenterX = -0.1f;
    float crankCenterY = 0.35f;
    float pedalRadius = 0.15f;
    
    float rightPedalX = crankCenterX + cosf(rightPedalAngle) * pedalRadius;
    float rightPedalY = crankCenterY + sinf(rightPedalAngle) * pedalRadius;
    
    float leftPedalX = crankCenterX + cosf(leftPedalAngle) * pedalRadius;
    float leftPedalY = crankCenterY + sinf(leftPedalAngle) * pedalRadius;
    
    // Right leg from hip to pedal
    glBegin(GL_LINES);
    glVertex3f(hipX + 0.05f, hipY, 0.0f);
    glVertex3f(rightPedalX, rightPedalY, 0.06f);
    glEnd();
    
    // Left leg from hip to pedal
    glBegin(GL_LINES);
    glVertex3f(hipX - 0.05f, hipY, 0.0f);
    glVertex3f(leftPedalX, leftPedalY, -0.06f);
    glEnd();
    
    // Cycling shoes - bright colorful
    glColor3f(1.0f, 0.4f, 0.35f);
    glLineWidth(10.0f);
    glBegin(GL_LINES);
    glVertex3f(rightPedalX - 0.05f, rightPedalY, 0.06f);
    glVertex3f(rightPedalX + 0.05f, rightPedalY, 0.06f);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(leftPedalX - 0.05f, leftPedalY, -0.06f);
    glVertex3f(leftPedalX + 0.05f, leftPedalY, -0.06f);
    glEnd();
    
    // Knee joints - metallic accent
    glColor3f(0.8f, 0.85f, 0.9f);
    glPointSize(6.0f);
    float rightKneeX = (hipX + 0.05f + rightPedalX) / 2.0f;
    float rightKneeY = (hipY + rightPedalY) / 2.0f + 0.08f;
    float leftKneeX = (hipX - 0.05f + leftPedalX) / 2.0f;
    float leftKneeY = (hipY + leftPedalY) / 2.0f + 0.08f;
    glBegin(GL_POINTS);
    glVertex3f(rightKneeX, rightKneeY, 0.03f);
    glVertex3f(leftKneeX, leftKneeY, -0.03f);
    glEnd();
    
    glEnable(GL_LIGHTING);
}

// Draw garage door
void drawGarageDoor(float openProgress) {
    glEnable(GL_LIGHTING);
    
    GLfloat doorDiffuse[] = { 0.6f, 0.6f, 0.65f, 1.0f };
    GLfloat doorSpecular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, doorDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, doorSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 32.0f);
    
    glPushMatrix();
    // Door starts at front of garage and lifts up
    float doorHeight = 2.5f;
    float doorWidth = 4.0f;
    float liftY = openProgress * 3.0f;  // Lifts up as it opens
    float rotateAngle = openProgress * 90.0f;  // Rotates up
    
    glTranslatef(0.0f, liftY, 3.0f);
    glTranslatef(0.0f, doorHeight / 2.0f, 0.0f);
    glRotatef(-rotateAngle, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, -doorHeight / 2.0f, 0.0f);
    
    // Door panels
    for (int i = 0; i < 5; i++) {
        float panelY = i * (doorHeight / 5.0f);
        glBegin(GL_QUADS);
        glNormal3f(0, 0, -1);
        glVertex3f(-doorWidth / 2.0f, panelY, 0.0f);
        glVertex3f(doorWidth / 2.0f, panelY, 0.0f);
        glVertex3f(doorWidth / 2.0f, panelY + doorHeight / 5.0f, 0.0f);
        glVertex3f(-doorWidth / 2.0f, panelY + doorHeight / 5.0f, 0.0f);
        glEnd();
    }
    glPopMatrix();
}

// Draw outdoor road
void drawRoad() {
    glEnable(GL_LIGHTING);
    
    // White-gray road - clean and bright
    GLfloat roadDiffuse[] = { 0.85f, 0.85f, 0.88f, 1.0f };
    GLfloat roadSpecular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, roadDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, roadSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 15.0f);
    
    // Main road/ground surface - extended to cover area visible from garage
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-20, 0, 3);
    glVertex3f(80, 0, 3);
    glVertex3f(80, 0, 60);
    glVertex3f(-20, 0, 60);
    glEnd();
    
    // Side areas - same white-gray color for consistency
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-20, 0, -20);
    glVertex3f(80, 0, -20);
    glVertex3f(80, 0, 3);
    glVertex3f(-20, 0, 3);
    glEnd();
    
    // Road lane markings - darker gray for contrast on white road
    GLfloat lineDiffuse[] = { 0.4f, 0.4f, 0.45f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lineDiffuse);
    
    for (float x = -5; x < 70; x += 3.0f) {
        glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glVertex3f(x, 0.01f, 8.5f);
        glVertex3f(x + 1.5f, 0.01f, 8.5f);
        glVertex3f(x + 1.5f, 0.01f, 9.5f);
        glVertex3f(x, 0.01f, 9.5f);
        glEnd();
    }
}

// Draw sky with sunset gradient
void drawSky() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);  // Draw sky behind everything
    
    // ===== 3D SKYDOME - covers entire background =====
    // This creates a half-sphere dome around the scene
    
    float radius = 100.0f;  // Large radius to encompass scene
    int segments = 32;
    int rings = 16;
    
    // Draw the sky dome with gradient colors
    for (int i = 0; i < rings; i++) {
        float lat0 = 3.14159f * (0.5f - (float)i / rings);
        float lat1 = 3.14159f * (0.5f - (float)(i + 1) / rings);
        
        float y0 = sinf(lat0);
        float y1 = sinf(lat1);
        float r0 = cosf(lat0);
        float r1 = cosf(lat1);
        
        // Color gradient based on height (y position)
        // Bottom = orange/pink horizon, top = deep blue/purple
        float t0 = (y0 + 1.0f) * 0.5f;  // 0 at bottom, 1 at top
        float t1 = (y1 + 1.0f) * 0.5f;
        
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= segments; j++) {
            float lng = 2.0f * 3.14159f * (float)j / segments;
            float x = cosf(lng);
            float z = sinf(lng);
            
            // Color for upper vertex
            float r_top = 1.0f - t0 * 0.6f;      // Orange to purple-blue
            float g_top = 0.5f + t0 * 0.1f - (1.0f - t0) * 0.1f;
            float b_top = 0.3f + t0 * 0.5f;
            glColor3f(r_top, g_top, b_top);
            glVertex3f(x * r0 * radius, y0 * radius * 0.5f + 5.0f, z * r0 * radius);
            
            // Color for lower vertex
            float r_bot = 1.0f - t1 * 0.6f;
            float g_bot = 0.5f + t1 * 0.1f - (1.0f - t1) * 0.1f;
            float b_bot = 0.3f + t1 * 0.5f;
            glColor3f(r_bot, g_bot, b_bot);
            glVertex3f(x * r1 * radius, y1 * radius * 0.5f + 5.0f, z * r1 * radius);
        }
        glEnd();
    }
    
    // Add a ground plane extension for the horizon (prevents black gaps)
    glColor3f(1.0f, 0.55f, 0.35f);  // Match horizon color
    glBegin(GL_QUADS);
    glVertex3f(-150, -2, -150);
    glVertex3f(150, -2, -150);
    glVertex3f(150, -2, 150);
    glVertex3f(-150, -2, 150);
    glEnd();
    
    glEnable(GL_DEPTH_TEST);
    
    // Soft glowing clouds with sunset colors
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Cloud 1 - peachy (front)
    glColor4f(1.0f, 0.85f, 0.7f, 0.6f);
    glPushMatrix();
    glTranslatef(-15.0f, 14.0f, -45.0f);
    glutSolidSphere(1.5f, 16, 16);
    glTranslatef(1.5f, 0.0f, 0.0f);
    glutSolidSphere(1.8f, 16, 16);
    glTranslatef(1.5f, -0.3f, 0.0f);
    glutSolidSphere(1.3f, 16, 16);
    glPopMatrix();
    
    // Cloud 2 - pinkish (front right)
    glColor4f(1.0f, 0.8f, 0.9f, 0.5f);
    glPushMatrix();
    glTranslatef(20.0f, 16.0f, -46.0f);
    glutSolidSphere(1.2f, 16, 16);
    glTranslatef(1.2f, 0.2f, 0.0f);
    glutSolidSphere(1.5f, 16, 16);
    glTranslatef(1.2f, 0.0f, 0.0f);
    glutSolidSphere(1.0f, 16, 16);
    glPopMatrix();
    
    // Cloud 3 - lavender (high)
    glColor4f(0.9f, 0.85f, 1.0f, 0.4f);
    glPushMatrix();
    glTranslatef(5.0f, 22.0f, -47.0f);
    glutSolidSphere(1.0f, 16, 16);
    glTranslatef(1.0f, 0.1f, 0.0f);
    glutSolidSphere(1.3f, 16, 16);
    glTranslatef(1.0f, -0.2f, 0.0f);
    glutSolidSphere(0.9f, 16, 16);
    glPopMatrix();
    
    // Additional clouds around the scene (sides and back)
    // Left side clouds
    glColor4f(1.0f, 0.75f, 0.6f, 0.5f);
    glPushMatrix();
    glTranslatef(-40.0f, 12.0f, 10.0f);
    glutSolidSphere(2.0f, 12, 12);
    glTranslatef(2.0f, 0.5f, 0.0f);
    glutSolidSphere(2.5f, 12, 12);
    glPopMatrix();
    
    // Right side clouds
    glColor4f(1.0f, 0.7f, 0.8f, 0.45f);
    glPushMatrix();
    glTranslatef(45.0f, 15.0f, -10.0f);
    glutSolidSphere(1.8f, 12, 12);
    glTranslatef(1.5f, -0.2f, 1.0f);
    glutSolidSphere(2.2f, 12, 12);
    glPopMatrix();
    
    // Back clouds
    glColor4f(0.95f, 0.8f, 0.85f, 0.4f);
    glPushMatrix();
    glTranslatef(10.0f, 18.0f, 40.0f);
    glutSolidSphere(2.5f, 12, 12);
    glTranslatef(3.0f, 0.0f, 0.0f);
    glutSolidSphere(2.0f, 12, 12);
    glPopMatrix();
    
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// Draw sun with golden hour effect
void drawSun() {
    glEnable(GL_LIGHTING);
    
    // Position for sunset - lower and more to the side
    float sunX = 25.0f;
    float sunY = 12.0f;
    float sunZ = -35.0f;
    
    // Sun glow (large outer layer for golden hour)
    GLfloat sunGlow[] = { 1.0f, 0.7f, 0.3f, 1.0f }; // Warm orange
    GLfloat sunEmission[] = { 1.0f, 0.6f, 0.2f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, sunGlow);
    glMaterialfv(GL_FRONT, GL_EMISSION, sunEmission);
    
    glPushMatrix();
    glTranslatef(sunX, sunY, sunZ);
    glutSolidSphere(3.5f, 32, 32);
    
    // Sun middle layer
    GLfloat sunMid[] = { 1.0f, 0.85f, 0.5f, 1.0f };
    GLfloat midEmission[] = { 1.0f, 0.8f, 0.4f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, sunMid);
    glMaterialfv(GL_FRONT, GL_EMISSION, midEmission);
    glutSolidSphere(2.8f, 32, 32);
    
    // Sun core (brightest)
    GLfloat sunCore[] = { 1.0f, 0.95f, 0.7f, 1.0f };
    GLfloat coreEmission[] = { 1.0f, 0.95f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, sunCore);
    glMaterialfv(GL_FRONT, GL_EMISSION, coreEmission);
    glutSolidSphere(2.2f, 32, 32);
    glPopMatrix();
    
    // Reset emission
    GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
}

// Draw tree
void drawTree(float scale) {
    glEnable(GL_LIGHTING);
    
    // Pink/magenta trunk to match Kick Buttowski style
    GLfloat trunkDiffuse[] = { 0.85f, 0.3f, 0.5f, 1.0f };
    GLfloat trunkSpecular[] = { 0.3f, 0.15f, 0.2f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, trunkDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, trunkSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 12.0f);
    
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.2f * scale, 0.12f * scale, 2.2f * scale, 12, 1);
    gluDeleteQuadric(quad);
    glPopMatrix();
    
    // ALL PINK foliage - Kick Buttowski vibrant style
    float colorVariation = (scale - 0.8f) * 0.5f;
    
    // Main canopy - hot pink
    GLfloat leaf1[] = { 1.0f, 0.3f + colorVariation, 0.6f, 1.0f };
    GLfloat leafSpec[] = { 0.5f, 0.3f, 0.4f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, leaf1);
    glMaterialfv(GL_FRONT, GL_SPECULAR, leafSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);
    
    glPushMatrix();
    glTranslatef(0.0f, 2.2f * scale, 0.0f);
    glutSolidSphere(1.0f * scale, 20, 20);
    
    // Secondary layer - deeper magenta
    GLfloat leaf2[] = { 0.9f, 0.2f, 0.55f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, leaf2);
    glTranslatef(-0.35f * scale, 0.5f * scale, 0.2f * scale);
    glutSolidSphere(0.7f * scale, 16, 16);
    
    // Top tuft - bright pink highlight
    GLfloat leaf3[] = { 1.0f, 0.5f, 0.7f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, leaf3);
    glTranslatef(0.7f * scale, 0.3f * scale, -0.1f * scale);
    glutSolidSphere(0.55f * scale, 14, 14);
    
    // Side cluster - rose pink
    GLfloat leaf4[] = { 0.95f, 0.35f, 0.6f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, leaf4);
    glTranslatef(-0.2f * scale, -0.6f * scale, 0.4f * scale);
    glutSolidSphere(0.5f * scale, 14, 14);
    glPopMatrix();
}

// Draw outdoor environment with golden hour lighting
void drawOutdoorEnvironment() {
    drawRoad();
    drawSky();
    drawSun();
    
    // Add warm golden hour ambient light
    GLfloat warmAmbient[] = { 0.6f, 0.5f, 0.4f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, warmAmbient);
    
    // Trees along the road - positioned away from garage area
    for (int i = 0; i < 8; i++) {
        float treeX = i * 6.0f + 5.0f;  // Start trees further right (past garage)
        
        // Trees on far side of road (z = -5, away from garage)
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, 0.2f);
        
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(treeX, 0.01f, -5.0f);
        for (int j = 0; j <= 20; j++) {
            float angle = j * 6.28318f / 20.0f;
            glVertex3f(treeX + cosf(angle) * 1.2f, 0.01f, -5.0f + sinf(angle) * 1.2f);
        }
        glEnd();
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
        
        glPushMatrix();
        glTranslatef(treeX, 0.0f, -5.0f);
        drawTree(1.0f);
        glPopMatrix();
        
        // Trees on near side of road (z = 20, far from garage)
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glColor4f(0.0f, 0.0f, 0.0f, 0.2f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(treeX, 0.01f, 20.0f);
        for (int j = 0; j <= 20; j++) {
            float angle = j * 6.28318f / 20.0f;
            glVertex3f(treeX + cosf(angle) * 1.0f, 0.01f, 20.0f + sinf(angle) * 1.0f);
        }
        glEnd();
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
        
        glPushMatrix();
        glTranslatef(treeX, 0.0f, 20.0f);
        drawTree(0.8f);
        glPopMatrix();
    }
}

// Draw park environment
void drawParkEnvironment() {
    glEnable(GL_LIGHTING);
    
    // Light lavender/white ground - Kick Buttowski clean style
    for (int x = -30; x < 60; x += 3) {
        for (int z = -30; z < 30; z += 3) {
            float distFromPath = fabsf((float)z);
            float intensity = 0.85f + (distFromPath / 120.0f) * 0.1f;
            float variation = sinf(x * 0.5f + z * 0.3f) * 0.03f;
            
            GLfloat grassTile[] = { 
                intensity + variation, 
                intensity - 0.05f + variation * 0.5f, 
                intensity + 0.02f + variation * 0.3f, 
                1.0f 
            };
            glMaterialfv(GL_FRONT, GL_DIFFUSE, grassTile);
            glMaterialf(GL_FRONT, GL_SHININESS, 8.0f);
            
            glBegin(GL_QUADS);
            glNormal3f(0, 1, 0);
            glVertex3f(x, 0, z);
            glVertex3f(x + 3, 0, z);
            glVertex3f(x + 3, 0, z + 3);
            glVertex3f(x, 0, z + 3);
            glEnd();
        }
    }
    
    // White-gray cobblestone path
    for (float px = -10; px < 60; px += 0.8f) {
        for (float pz = -2; pz < 2; pz += 0.8f) {
            float shade = 0.75f + sinf(px * 3.0f + pz * 2.0f) * 0.1f;
            GLfloat stone[] = { shade + 0.05f, shade + 0.03f, shade + 0.05f, 1.0f };
            glMaterialfv(GL_FRONT, GL_DIFFUSE, stone);
            
            glBegin(GL_QUADS);
            glNormal3f(0, 1, 0);
            glVertex3f(px, 0.02f, pz);
            glVertex3f(px + 0.7f, 0.02f, pz);
            glVertex3f(px + 0.7f, 0.02f, pz + 0.7f);
            glVertex3f(px, 0.02f, pz + 0.7f);
            glEnd();
        }
    }
    
    // Path edge flowers (small colorful dots)
    glDisable(GL_LIGHTING);
    glPointSize(6.0f);
    for (float fx = -8; fx < 55; fx += 2.5f) {
        float hue = fmodf(fx * 0.3f, 1.0f);
        glColor3f(0.7f + 0.3f * sinf(hue * 6.28f),
                  0.5f + 0.5f * sinf(hue * 6.28f + 2.1f),
                  0.5f + 0.5f * sinf(hue * 6.28f + 4.2f));
        glBegin(GL_POINTS);
        glVertex3f(fx + sinf(fx) * 0.3f, 0.1f, -2.5f + cosf(fx * 0.7f) * 0.3f);
        glVertex3f(fx + cosf(fx) * 0.3f, 0.1f, 2.5f + sinf(fx * 0.5f) * 0.3f);
        glEnd();
    }
    glEnable(GL_LIGHTING);
    
    // Scattered trees in park
    float treePositions[][2] = {
        {-5, -8}, {3, -10}, {12, -12}, {22, -9},
        {-3, 8}, {8, 10}, {18, 12}, {28, 9},
        {35, -11}, {35, 11}, {45, -8}, {45, 8}
    };
    
    for (int i = 0; i < 12; i++) {
        glPushMatrix();
        glTranslatef(treePositions[i][0], 0.0f, treePositions[i][1]);
        drawTree(0.9f + (i % 3) * 0.2f);
        glPopMatrix();
    }
    
    // Vibrant flower clusters with multiple layers
    float flowerPositions[][3] = {
        {-2, -5, 0}, {5, -6, 1}, {15, -7, 2}, {25, -6, 0},
        {0, 6, 1}, {10, 7, 2}, {20, 8, 0}, {30, 6, 1},
        {40, -5, 3}, {50, 7, 2}
    };
    
    float flowerColors[][3] = {
        {1.0f, 0.3f, 0.5f},   // Hot pink
        {1.0f, 0.5f, 0.7f},   // Light pink
        {0.9f, 0.2f, 0.6f},   // Magenta
        {1.0f, 0.4f, 0.55f}   // Rose
    };
    
    for (int i = 0; i < 10; i++) {
        glPushMatrix();
        glTranslatef(flowerPositions[i][0], 0.0f, flowerPositions[i][1]);
        
        int colorIdx = (int)flowerPositions[i][2] % 4;
        
        // Leaves underneath
        GLfloat leafGreen[] = { 0.2f, 0.6f, 0.25f, 1.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, leafGreen);
        glutSolidSphere(0.35f, 10, 10);
        
        // Flower blooms on top
        GLfloat flowerDiffuse[] = { 
            flowerColors[colorIdx][0], 
            flowerColors[colorIdx][1], 
            flowerColors[colorIdx][2], 
            1.0f 
        };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, flowerDiffuse);
        
        glTranslatef(0.0f, 0.25f, 0.0f);
        glutSolidSphere(0.3f, 12, 12);
        glTranslatef(0.25f, 0.05f, 0.1f);
        glutSolidSphere(0.22f, 10, 10);
        glTranslatef(-0.5f, 0.0f, 0.15f);
        glutSolidSphere(0.25f, 10, 10);
        glPopMatrix();
    }
    
    // Sky backdrop
    drawSky();
    drawSun();
}

// ===== KICK BUTTOWSKI STYLE EFFECTS =====

// Draw speed lines behind moving object (action lines)
void drawSpeedLines(float x, float y, float z, float intensity, float direction) {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    int numLines = 15 + (int)(intensity * 10);
    
    for (int i = 0; i < numLines; i++) {
        float offsetY = (i - numLines / 2.0f) * 0.12f;
        float offsetZ = sinf(i * 0.7f) * 0.3f;
        float lineLength = 1.0f + intensity * 2.5f + (i % 3) * 0.3f;
        float alpha = 0.7f - fabsf(offsetY) * 0.15f;
        
        // Gradient from white to orange/yellow (hot speed effect)
        float hue = (float)i / numLines;
        glColor4f(1.0f, 0.8f - hue * 0.3f, 0.3f - hue * 0.2f, alpha * intensity);
        
        glLineWidth(2.0f + (numLines - i) * 0.15f);
        glBegin(GL_LINES);
        glVertex3f(x, y + offsetY, z + offsetZ);
        glVertex3f(x - lineLength * direction, y + offsetY + offsetY * 0.1f, z + offsetZ);
        glEnd();
    }
    
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// Draw impact burst effect (for dramatic moments)
void drawImpactBurst(float x, float y, float z, float size, float phase) {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    int numRays = 16;
    float pulseSize = size * (1.0f + sinf(phase * 8.0f) * 0.3f);
    
    for (int i = 0; i < numRays; i++) {
        float angle = i * 6.28318f / numRays + phase * 0.5f;
        float rayLength = pulseSize * (0.8f + (i % 2) * 0.4f);
        
        // Hot yellow to white gradient
        float alpha = 0.8f - (i % 3) * 0.15f;
        glColor4f(1.0f, 0.95f - (i % 2) * 0.2f, 0.4f, alpha);
        
        glLineWidth(4.0f - (i % 3));
        glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x + cosf(angle) * rayLength, y + sinf(angle) * rayLength, z);
        glEnd();
    }
    
    // Center glow
    glColor4f(1.0f, 1.0f, 0.9f, 0.9f);
    glPointSize(size * 15.0f);
    glBegin(GL_POINTS);
    glVertex3f(x, y, z);
    glEnd();
    
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// Draw action zoom lines (radial speed effect)
void drawZoomLines(float centerX, float centerY, float intensity) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Save current matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    int numLines = 32;
    for (int i = 0; i < numLines; i++) {
        float angle = i * 6.28318f / numLines;
        float innerRadius = 0.3f + (i % 3) * 0.1f;
        float outerRadius = 1.5f;
        float alpha = intensity * (0.4f + (i % 2) * 0.2f);
        
        glColor4f(1.0f, 0.9f, 0.7f, alpha);
        glLineWidth(1.5f + (i % 2));
        glBegin(GL_LINES);
        glVertex2f(cosf(angle) * innerRadius, sinf(angle) * innerRadius);
        glVertex2f(cosf(angle) * outerRadius, sinf(angle) * outerRadius);
        glEnd();
    }
    
    // Restore matrices
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// Draw dust cloud effect
void drawDustCloud(float x, float y, float z, float size, float age) {
    glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    float alpha = fmaxf(0.0f, 0.7f - age * 0.3f);
    float expandedSize = size * (1.0f + age * 0.8f);
    
    GLfloat dustColor[] = { 0.85f, 0.75f, 0.6f, alpha };
    GLfloat dustEmit[] = { 0.1f, 0.08f, 0.05f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, dustColor);
    glMaterialfv(GL_FRONT, GL_EMISSION, dustEmit);
    
    // Multiple puff layers
    for (int i = 0; i < 5; i++) {
        float offsetX = sinf(i * 1.2f + age) * expandedSize * 0.4f;
        float offsetY = fabsf(cosf(i * 0.8f)) * expandedSize * 0.3f + age * 0.2f;
        float offsetZ = cosf(i * 1.5f) * expandedSize * 0.3f;
        float puffSize = expandedSize * (0.3f + (i % 3) * 0.15f);
        
        glPushMatrix();
        glTranslatef(x + offsetX, y + offsetY, z + offsetZ);
        glutSolidSphere(puffSize, 8, 8);
        glPopMatrix();
    }
    
    GLfloat noEmit[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmit);
    glDisable(GL_BLEND);
}

// Draw stickman doing a wheelie (Kick Buttowski action pose)
void drawStickmanWheelie(float pedalPhase, float helmetHue, float wheelieAngle) {
    glDisable(GL_LIGHTING);
    
    // Rotate the whole figure for wheelie
    glPushMatrix();
    glRotatef(wheelieAngle, 0, 0, 1);
    
    float seatX = -0.3f;
    float seatY = 0.7f;
    
    float hipX = seatX;
    float hipY = seatY;
    float shoulderX = hipX + 0.1f;
    float shoulderY = hipY + 0.35f;
    float headX = shoulderX + 0.03f;
    float headY = shoulderY + 0.22f;
    
    float rightPedalAngle = pedalPhase;
    float leftPedalAngle = pedalPhase + 3.14159f;
    
    // Head - bigger for cartoon style
    glColor3f(0.95f, 0.78f, 0.65f);
    glPointSize(20.0f);
    glBegin(GL_POINTS);
    glVertex3f(headX, headY, 0.0f);
    glEnd();
    
    // Excited eyes (wider)
    glColor3f(0.1f, 0.1f, 0.15f);
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    glVertex3f(headX + 0.015f, headY + 0.03f, 0.05f);
    glVertex3f(headX + 0.055f, headY + 0.03f, 0.05f);
    glEnd();
    
    // Big grin
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 8; i++) {
        float angle = 3.14159f + i * 3.14159f / 8.0f;
        glVertex3f(headX + 0.035f + cosf(angle) * 0.03f, headY - 0.02f + sinf(angle) * 0.02f, 0.05f);
    }
    glEnd();
    
    // BOLD helmet with racing stripe
    float helmetR = 0.95f * helmetHue + 0.3f * (1.0f - helmetHue);
    float helmetG = 0.4f + 0.4f * helmetHue;
    float helmetB = 0.85f * (1.0f - helmetHue * 0.3f);
    glColor3f(helmetR, helmetG, helmetB);
    glLineWidth(6.0f);
    float helmetRadius = 0.13f;
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 14; i++) {
        float angle = i * 3.14159f / 14.0f;
        glVertex3f(headX + cosf(angle) * helmetRadius, headY + sinf(angle) * helmetRadius, 0.0f);
    }
    glEnd();
    
    // Racing stripe on helmet
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.5f);
    glBegin(GL_LINE_STRIP);
    for (int i = 2; i <= 12; i++) {
        float angle = i * 3.14159f / 14.0f;
        glVertex3f(headX + cosf(angle) * (helmetRadius - 0.02f), headY + sinf(angle) * (helmetRadius - 0.02f), 0.01f);
    }
    glEnd();
    
    // Body - leaning back aggressively
    glColor3f(0.3f + helmetHue * 0.4f, 0.5f, 0.7f - helmetHue * 0.3f);
    glLineWidth(11.0f);
    glBegin(GL_LINES);
    glVertex3f(shoulderX, shoulderY, 0.0f);
    glVertex3f(hipX, hipY, 0.0f);
    glEnd();
    
    // Arms thrown back for balance!
    glColor3f(0.95f, 0.78f, 0.65f);
    glLineWidth(7.0f);
    
    float handlebarX = 0.4f;
    float handlebarY = 0.7f;
    
    glBegin(GL_LINES);
    glVertex3f(shoulderX + 0.05f, shoulderY, 0.0f);
    glVertex3f(handlebarX, handlebarY + 0.1f, 0.1f);  // Raised grip
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(shoulderX - 0.05f, shoulderY, 0.0f);
    glVertex3f(handlebarX, handlebarY + 0.1f, -0.1f);
    glEnd();
    
    // Power gloves
    glColor3f(1.0f, 0.3f, 0.2f);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glVertex3f(handlebarX, handlebarY + 0.1f, 0.1f);
    glVertex3f(handlebarX, handlebarY + 0.1f, -0.1f);
    glEnd();
    
    // Legs pumping the pedals
    glColor3f(0.15f, 0.2f, 0.28f);
    glLineWidth(8.0f);
    
    float crankCenterX = -0.1f;
    float crankCenterY = 0.35f;
    float pedalRadius = 0.15f;
    
    float rightPedalX = crankCenterX + cosf(rightPedalAngle) * pedalRadius;
    float rightPedalY = crankCenterY + sinf(rightPedalAngle) * pedalRadius;
    float leftPedalX = crankCenterX + cosf(leftPedalAngle) * pedalRadius;
    float leftPedalY = crankCenterY + sinf(leftPedalAngle) * pedalRadius;
    
    glBegin(GL_LINES);
    glVertex3f(hipX + 0.05f, hipY, 0.0f);
    glVertex3f(rightPedalX, rightPedalY, 0.06f);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(hipX - 0.05f, hipY, 0.0f);
    glVertex3f(leftPedalX, leftPedalY, -0.06f);
    glEnd();
    
    // Power sneakers
    glColor3f(1.0f, 0.35f, 0.25f);
    glLineWidth(12.0f);
    glBegin(GL_LINES);
    glVertex3f(rightPedalX - 0.06f, rightPedalY, 0.06f);
    glVertex3f(rightPedalX + 0.06f, rightPedalY, 0.06f);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(leftPedalX - 0.06f, leftPedalY, -0.06f);
    glVertex3f(leftPedalX + 0.06f, leftPedalY, -0.06f);
    glEnd();
    
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// Draw stickman with fist pump (victory pose)
void drawStickmanVictory(float phase, float helmetHue) {
    glDisable(GL_LIGHTING);
    
    float hipY = 0.9f;
    float shoulderY = 1.3f;
    float headY = 1.55f;
    float bounce = fabsf(sinf(phase * 4.0f)) * 0.05f;
    
    // Head
    glColor3f(0.95f, 0.78f, 0.65f);
    glPointSize(22.0f);
    glBegin(GL_POINTS);
    glVertex3f(0.0f, headY + bounce, 0.0f);
    glEnd();
    
    // Happy squint eyes
    glColor3f(0.1f, 0.1f, 0.15f);
    glLineWidth(2.5f);
    glBegin(GL_LINES);
    glVertex3f(-0.03f, headY + bounce + 0.02f, 0.05f);
    glVertex3f(-0.01f, headY + bounce + 0.04f, 0.05f);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0.01f, headY + bounce + 0.04f, 0.05f);
    glVertex3f(0.03f, headY + bounce + 0.02f, 0.05f);
    glEnd();
    
    // Big triumphant grin
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.5f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 10; i++) {
        float angle = 3.14159f + i * 3.14159f / 10.0f;
        glVertex3f(cosf(angle) * 0.04f, headY + bounce - 0.03f + sinf(angle) * 0.025f, 0.05f);
    }
    glEnd();
    
    // Bold helmet
    float helmetR = 0.95f * helmetHue + 0.3f * (1.0f - helmetHue);
    float helmetG = 0.4f + 0.4f * helmetHue;
    float helmetB = 0.85f * (1.0f - helmetHue * 0.3f);
    glColor3f(helmetR, helmetG, helmetB);
    glLineWidth(6.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 14; i++) {
        float angle = i * 3.14159f / 14.0f;
        glVertex3f(cosf(angle) * 0.12f, headY + bounce + sinf(angle) * 0.12f, 0.0f);
    }
    glEnd();
    
    // Body
    glColor3f(0.3f + helmetHue * 0.4f, 0.5f, 0.7f - helmetHue * 0.3f);
    glLineWidth(11.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, shoulderY + bounce, 0.0f);
    glVertex3f(0.0f, hipY + bounce, 0.0f);
    glEnd();
    
    // RIGHT ARM - FIST PUMP UP!
    glColor3f(0.95f, 0.78f, 0.65f);
    glLineWidth(7.0f);
    float pumpHeight = 0.25f + sinf(phase * 6.0f) * 0.1f;
    glBegin(GL_LINES);
    glVertex3f(0.0f, shoulderY + bounce, 0.0f);
    glVertex3f(0.15f, shoulderY + bounce + 0.15f, 0.0f);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0.15f, shoulderY + bounce + 0.15f, 0.0f);
    glVertex3f(0.1f, shoulderY + bounce + pumpHeight + 0.2f, 0.0f);
    glEnd();
    
    // Power fist!
    glColor3f(1.0f, 0.3f, 0.2f);
    glPointSize(14.0f);
    glBegin(GL_POINTS);
    glVertex3f(0.1f, shoulderY + bounce + pumpHeight + 0.22f, 0.0f);
    glEnd();
    
    // Left arm relaxed
    glColor3f(0.95f, 0.78f, 0.65f);
    glLineWidth(6.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, shoulderY + bounce, 0.0f);
    glVertex3f(-0.2f, shoulderY + bounce - 0.1f, 0.0f);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(-0.2f, shoulderY + bounce - 0.1f, 0.0f);
    glVertex3f(-0.25f, shoulderY + bounce - 0.25f, 0.0f);
    glEnd();
    
    // Legs in power stance
    glColor3f(0.15f, 0.2f, 0.28f);
    glLineWidth(9.0f);
    glBegin(GL_LINES);
    glVertex3f(0.05f, hipY + bounce, 0.0f);
    glVertex3f(0.15f, 0.5f + bounce, 0.0f);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0.15f, 0.5f + bounce, 0.0f);
    glVertex3f(0.2f, 0.0f, 0.0f);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(-0.05f, hipY + bounce, 0.0f);
    glVertex3f(-0.12f, 0.5f + bounce, 0.0f);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(-0.12f, 0.5f + bounce, 0.0f);
    glVertex3f(-0.1f, 0.0f, 0.0f);
    glEnd();
    
    // Power sneakers
    glColor3f(1.0f, 0.35f, 0.25f);
    glLineWidth(12.0f);
    glBegin(GL_LINES);
    glVertex3f(0.2f, 0.0f, 0.0f);
    glVertex3f(0.33f, 0.0f, 0.0f);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(-0.1f, 0.0f, 0.0f);
    glVertex3f(0.03f, 0.0f, 0.0f);
    glEnd();
    
    glEnable(GL_LIGHTING);
}

// Draw bold action text (for intro style)
void drawActionText(const char* text, float x, float y, float size, float r, float g, float b) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    // Draw outline first (black)
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(4.0f);
    glPushMatrix();
    glTranslatef(x + 0.02f, y - 0.02f, 0.0f);
    glScalef(size * 0.001f, size * 0.001f, 1.0f);
    for (const char* c = text; *c; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();
    
    // Main text
    glColor3f(r, g, b);
    glLineWidth(3.0f);
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(size * 0.001f, size * 0.001f, 1.0f);
    for (const char* c = text; *c; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}
