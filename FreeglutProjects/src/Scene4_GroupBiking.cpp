#include "Scene4_GroupBiking.h"

Scene4_GroupBiking::Scene4_GroupBiking() 
    : groupBikeProgress(0.0f), groupBikeSpeed(0.04f), walkPhase(0.0f), sceneTimer(0.0f) {
}

void Scene4_GroupBiking::onEnter() {
    groupBikeProgress = 0.0f;
    walkPhase = 0.0f;
    sceneTimer = 0.0f;
    manualCameraControl = false;
}

void Scene4_GroupBiking::update(float deltaTime) {
    sceneTimer += deltaTime;
    
    // FAST group biking - Kick Buttowski style!
    if (groupBikeProgress < 25.0f) {
        // Speed varies - accelerate, cruise, accelerate!
        float speedBoost = 1.0f + sinf(sceneTimer * 2.0f) * 0.3f;
        groupBikeProgress += groupBikeSpeed * speedBoost;
        walkPhase += 0.35f * speedBoost;  // Fast pedaling!
    }
}

void Scene4_GroupBiking::render() {
    drawParkEnvironment();
    
    float helmetHues[5] = {0.9f, 0.3f, 0.6f, 1.0f, 0.15f};
    
    // V-Formation like a bike gang!
    float formations[5][2] = {
        {0.0f, 0.0f},      // Leader (main char)
        {-1.2f, -0.6f},    // Left wing
        {1.2f, -0.6f},     // Right wing  
        {-2.0f, -1.2f},    // Far left
        {2.0f, -1.2f}      // Far right
    };
    
    for (int i = 0; i < 5; i++) {
        float xOffset = groupBikeProgress + formations[i][0];
        float zOffset = formations[i][1];
        
        // Motion blur shadows
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Stretched shadow for speed effect
        glColor4f(0.0f, 0.0f, 0.0f, 0.2f);
        glBegin(GL_QUADS);
        glVertex3f(xOffset - 1.5f, 0.01f, zOffset - 0.3f);
        glVertex3f(xOffset + 0.5f, 0.01f, zOffset - 0.3f);
        glVertex3f(xOffset + 0.5f, 0.01f, zOffset + 0.3f);
        glVertex3f(xOffset - 1.5f, 0.01f, zOffset + 0.3f);
        glEnd();
        
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
        
        glPushMatrix();
        glTranslatef(xOffset, 0.0f, zOffset);
        
        // Leader does wheelies occasionally!
        if (i == 0 && fmodf(sceneTimer, 4.0f) < 1.5f) {
            float wheeliePhase = fmodf(sceneTimer, 4.0f) / 1.5f;
            float wheelieAngle = sinf(wheeliePhase * 3.14159f) * 15.0f;
            glRotatef(-wheelieAngle, 0, 0, 1);
            drawBike();
            drawStickmanWheelie(walkPhase, helmetHues[i], wheelieAngle * 0.5f);
        } else {
            drawBike();
            drawStickmanOnBike(walkPhase + i * 0.4f, helmetHues[i]);
        }
        glPopMatrix();
    }
    
    // SPEED LINES behind the whole group!
    float speedIntensity = 0.6f + sinf(sceneTimer * 2.0f) * 0.2f;
    drawSpeedLines(groupBikeProgress - 2.5f, 0.8f, 0.0f, speedIntensity, 1.0f);
    drawSpeedLines(groupBikeProgress - 3.0f, 0.5f, -0.8f, speedIntensity * 0.7f, 1.0f);
    drawSpeedLines(groupBikeProgress - 3.0f, 0.5f, 0.8f, speedIntensity * 0.7f, 1.0f);
    
    // Dust trail
    if (fmodf(sceneTimer, 0.3f) < 0.15f) {
        float dustX = groupBikeProgress - 3.5f - fmodf(sceneTimer * 5.0f, 2.0f);
        drawDustCloud(dustX, 0.1f, 0.0f, 0.3f, fmodf(sceneTimer, 0.5f));
    }
    
    // Zoom lines during speed bursts
    float speedBurst = sinf(sceneTimer * 2.0f);
    if (speedBurst > 0.7f) {
        drawZoomLines(0, 0, (speedBurst - 0.7f) * 1.5f);
    }
}

void Scene4_GroupBiking::setupCamera() {
    float t = sceneTimer;
    
    // KICK BUTTOWSKI: Multiple dynamic camera angles
    float cycleTime = fmodf(t, 8.0f);
    
    if (cycleTime < 2.0f) {
        // LOW ANGLE HERO SHOT - looking up at the group
        float camX = groupBikeProgress - 3.0f;
        float shake = sinf(t * 20.0f) * 0.02f;
        gluLookAt(camX, 0.4f + shake, 5.0f,
                 groupBikeProgress + 1.0f, 1.5f, 0.0f,
                 shake, 1.0, 0.0);
    } else if (cycleTime < 4.0f) {
        // CHASE CAM - behind and to the side
        float camX = groupBikeProgress - 5.0f;
        float camZ = 3.0f + sinf(t * 0.5f) * 2.0f;
        float shake = sinf(t * 18.0f) * 0.03f;
        gluLookAt(camX, 1.8f + shake, camZ,
                 groupBikeProgress, 1.0f, 0.0f,
                 0.0, 1.0, 0.0);
    } else if (cycleTime < 5.5f) {
        // EXTREME CLOSE-UP on wheels/pedals
        float camX = groupBikeProgress - 0.5f;
        gluLookAt(camX, 0.5f, 1.5f,
                 groupBikeProgress, 0.3f, 0.0f,
                 0.0, 1.0, 0.0);
    } else if (cycleTime < 6.5f) {
        // FRONT ANGLE - coming at camera
        float camX = groupBikeProgress + 6.0f;
        gluLookAt(camX, 1.5f, 0.0f,
                 groupBikeProgress, 1.0f, 0.0f,
                 0.0, 1.0, 0.0);
    } else {
        // AERIAL/BIRD'S EYE tracking shot
        float camX = groupBikeProgress - 2.0f;
        float camHeight = 5.0f + sinf(t * 0.3f) * 1.0f;
        gluLookAt(camX, camHeight, 8.0f,
                 groupBikeProgress, 0.5f, 0.0f,
                 0.0, 1.0, 0.0);
    }
}

bool Scene4_GroupBiking::isComplete() {
    return groupBikeProgress >= 25.0f;
}
