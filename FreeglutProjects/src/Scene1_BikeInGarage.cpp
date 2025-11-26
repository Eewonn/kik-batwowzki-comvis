#include "Scene1_BikeInGarage.h"

Scene1_BikeInGarage::Scene1_BikeInGarage() 
    : stickmanX(-3.5f), bikeX(1.8f), sceneTimer(0.0f),
      walkPhase(0.0f), pedalPhase(0.0f), isMounted(false),
      isRidingOut(false), doorProgress(0.0f) {
}

void Scene1_BikeInGarage::onEnter() {
    stickmanX = -3.5f;
    sceneTimer = 0.0f;
    walkPhase = 0.0f;
    pedalPhase = 0.0f;
    isMounted = false;
    isRidingOut = false;
    doorProgress = 0.0f;
    manualCameraControl = false;
    cameraDistance = 8.0f;
    cameraAngleY = 0.0f;
    cameraAngleX = 20.0f;
    cameraTargetX = 0.0f;
    cameraTargetY = 1.0f;
    cameraTargetZ = 0.0f;
}

void Scene1_BikeInGarage::update(float deltaTime) {
    sceneTimer += deltaTime;
    
    if (!isMounted) {
        // Walk to bike
        if (stickmanX < bikeX - 0.5f) {
            stickmanX += 0.025f;
            walkPhase += 0.2f;
        } else {
            // Mount the bike
            isMounted = true;
            stickmanX = bikeX;
        }
    } else if (!isRidingOut) {
        // Open door
        if (doorProgress < 1.0f) {
            doorProgress += 0.015f;
        } else {
            doorProgress = 1.0f;
            isRidingOut = true;
        }
    } else {
        // Ride out!
        stickmanX += 0.04f;
        pedalPhase += 0.3f;
    }
}

void Scene1_BikeInGarage::render() {
    // Bright lighting for pink garage
    GLfloat brightAmbient[] = { 0.7f, 0.6f, 0.65f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, brightAmbient);
    
    GLfloat brightLight[] = { 1.0f, 0.95f, 0.9f, 1.0f };
    GLfloat lightPos[] = { 0.0f, 5.0f, 3.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, brightLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    // Draw the full 3D outdoor environment behind garage (visible through door)
    drawSky();  // 3D skybox
    drawSun();
    
    // Draw a large ground plane outside the garage (same color as road - white/gray)
    glEnable(GL_LIGHTING);
    GLfloat groundDiffuse[] = { 0.85f, 0.85f, 0.88f, 1.0f };  // Same as road color
    GLfloat groundSpec[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, groundDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, groundSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, 15.0f);
    
    // Ground covers everything outside the garage
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-30, -0.01f, 3);    // From garage front edge
    glVertex3f(80, -0.01f, 3);
    glVertex3f(80, -0.01f, 100);
    glVertex3f(-30, -0.01f, 100);
    glEnd();
    
    // Draw some trees visible through the door opening
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glTranslatef(i * 5.0f + 8.0f, 0.0f, 8.0f + (i % 2) * 3.0f);
        drawTree(0.8f + (i % 3) * 0.2f);
        glPopMatrix();
    }
    
    // Draw garage (will cover the outdoor scene except through the door)
    if (stickmanX < 8.0f) {
        drawGarage();
        
        // Draw door if mounting or riding
        if (isMounted) {
            drawGarageDoor(doorProgress);
        }
    }
    
    if (isMounted) {
        glPushMatrix();
        glTranslatef(stickmanX, 0.0f, 0.0f);
        
        if (isRidingOut && stickmanX > bikeX + 1.0f) {
            // Wheelie when blasting out!
            float wheelieProgress = fminf(1.0f, (stickmanX - bikeX - 1.0f) * 0.4f);
            float wheelieAngle = wheelieProgress * 18.0f;
            glRotatef(-wheelieAngle, 0, 0, 1);
            drawBike();
            drawStickmanWheelie(pedalPhase, 0.9f, wheelieAngle * 0.4f);
            
            // Speed lines!
            drawSpeedLines(stickmanX - 0.5f, 0.6f, 0.0f, wheelieProgress, 1.0f);
        } else {
            drawBike();
            drawStickmanOnBike(pedalPhase, 0.9f);
        }
        glPopMatrix();
    } else {
        // Walking
        glPushMatrix();
        glTranslatef(stickmanX, 0.0f, 0.0f);
        drawStickman(walkPhase, 0.9f);
        glPopMatrix();
        
        // Static bike
        glPushMatrix();
        glTranslatef(bikeX, 0.0f, 0.0f);
        drawBike();
        glPopMatrix();
    }
    
    // Zoom lines at start
    if (sceneTimer < 1.5f) {
        float zoomIntensity = fmaxf(0.0f, 1.0f - sceneTimer * 0.7f);
        drawZoomLines(0, 0, zoomIntensity * 0.5f);
    }
    
    // Impact burst when door opens
    if (doorProgress > 0.9f && doorProgress < 1.0f) {
        drawImpactBurst(0.0f, 1.5f, 3.5f, 1.2f, sceneTimer);
    }
}

void Scene1_BikeInGarage::setupCamera() {
    float t = sceneTimer;
    
    if (!isMounted) {
        // Track the walk
        float camX = stickmanX + 2.5f;
        gluLookAt(camX, 1.2f, 5.0f,
                 stickmanX, 1.0f, 0.0f,
                 0.0, 1.0, 0.0);
    } else if (!isRidingOut) {
        // Anticipation - look at door
        gluLookAt(stickmanX - 1.5f, 1.5f, 2.5f,
                 0.0f, 1.5f, 3.0f,
                 0.0, 1.0, 0.0);
    } else {
        // Chase cam riding out
        float speed = fminf(1.0f, (stickmanX - bikeX) * 0.2f);
        float shake = sinf(t * 20.0f) * speed * 0.04f;
        gluLookAt(stickmanX - 4.0f, 1.3f + shake, 4.5f,
                 stickmanX + 1.0f, 0.9f, 0.0f,
                 shake * 0.3f, 1.0, 0.0);
    }
}

bool Scene1_BikeInGarage::isComplete() {
    return isRidingOut && stickmanX >= 12.0f;
}
