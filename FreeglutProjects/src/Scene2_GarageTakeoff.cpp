#include "Scene2_GarageTakeoff.h"

Scene2_GarageTakeoff::Scene2_GarageTakeoff() 
    : stickmanX(-5.0f), bikeX(0.0f), walkSpeed(0.045f), 
      walkPhase(0.0f), pedalPhase(0.0f), walkingStarted(false), 
      isMounted(true), doorOpening(false), doorFullyOpen(true), 
      doorProgress(1.0f), sceneTimer(0.0f) {
}

void Scene2_GarageTakeoff::onEnter() {
    stickmanX = -5.0f;
    sceneTimer = 0.0f;
    walkingStarted = true;
    isMounted = true;  // Already on bike
    doorOpening = false;
    doorFullyOpen = true;
    doorProgress = 1.0f;
    walkPhase = 0.0f;
    pedalPhase = 0.0f;
    manualCameraControl = false;
}

void Scene2_GarageTakeoff::update(float deltaTime) {
    sceneTimer += deltaTime;
    
    // Riding through the park to meet friends
    if (stickmanX < 20.0f) {
        // Variable speed - accelerate and cruise
        float speedBoost = 1.0f + sinf(sceneTimer * 1.5f) * 0.25f;
        stickmanX += walkSpeed * speedBoost;
        pedalPhase += 0.3f * speedBoost;
    }
}

void Scene2_GarageTakeoff::render() {
    // Draw park environment - riding to meet friends
    drawParkEnvironment();
    
    // Main character riding fast through park
    glPushMatrix();
    glTranslatef(stickmanX, 0.0f, 0.0f);
    
    // Occasional wheelies for style!
    float wheelieCycle = fmodf(sceneTimer, 3.0f);
    if (wheelieCycle < 1.0f && stickmanX > 0.0f) {
        float wheeliePhase = sinf(wheelieCycle * 3.14159f);
        float wheelieAngle = wheeliePhase * 15.0f;
        glRotatef(-wheelieAngle, 0, 0, 1);
        drawBike();
        drawStickmanWheelie(pedalPhase, 0.9f, wheelieAngle * 0.4f);
    } else {
        drawBike();
        drawStickmanOnBike(pedalPhase, 0.9f);
    }
    glPopMatrix();
    
    // Speed lines behind rider
    float speedIntensity = 0.5f + sinf(sceneTimer * 2.0f) * 0.2f;
    drawSpeedLines(stickmanX - 0.8f, 0.7f, 0.0f, speedIntensity, 1.0f);
    
    // Dust trail occasionally
    if (fmodf(sceneTimer, 0.5f) < 0.2f) {
        drawDustCloud(stickmanX - 2.0f, 0.1f, 0.0f, 0.25f, fmodf(sceneTimer, 0.5f));
    }
    
    // Zoom lines during speed bursts
    float speedBurst = sinf(sceneTimer * 1.5f);
    if (speedBurst > 0.8f) {
        drawZoomLines(0, 0, (speedBurst - 0.8f) * 2.0f);
    }
}

void Scene2_GarageTakeoff::setupCamera() {
    float t = sceneTimer;
    float cycleTime = fmodf(t, 6.0f);
    
    if (cycleTime < 2.0f) {
        // Side tracking shot
        float camX = stickmanX - 2.0f;
        float shake = sinf(t * 18.0f) * 0.025f;
        gluLookAt(camX, 1.0f + shake, 5.0f,
                 stickmanX + 1.0f, 0.9f, 0.0f,
                 0.0, 1.0, 0.0);
    } else if (cycleTime < 3.5f) {
        // Low angle hero shot
        float camX = stickmanX - 3.0f;
        gluLookAt(camX, 0.4f, 4.0f,
                 stickmanX + 0.5f, 1.2f, 0.0f,
                 0.0, 1.0, 0.0);
    } else if (cycleTime < 5.0f) {
        // Chase cam from behind
        float camX = stickmanX - 5.0f;
        float camZ = 2.0f + sinf(t * 0.5f) * 1.5f;
        gluLookAt(camX, 1.5f, camZ,
                 stickmanX, 1.0f, 0.0f,
                 0.0, 1.0, 0.0);
    } else {
        // Wide establishing shot
        gluLookAt(stickmanX - 2.0f, 3.0f, 8.0f,
                 stickmanX + 2.0f, 0.8f, 0.0f,
                 0.0, 1.0, 0.0);
    }
}

bool Scene2_GarageTakeoff::isComplete() {
    return stickmanX >= 20.0f;
}
