#include "Scene3_MeetingFriends.h"

Scene3_MeetingFriends::Scene3_MeetingFriends() : stickmanX(-8.0f), sceneTimer(0.0f) {
    // Friends positioned waiting - they'll wave and join
    friends[0] = {3.0f, -2.0f, 0.0f, 0.0f};
    friends[1] = {4.5f, -2.5f, 0.5f, 0.0f};
    friends[2] = {6.0f, -2.0f, 1.0f, 0.0f};
    friends[3] = {7.5f, -2.5f, 1.5f, 0.0f};
}

void Scene3_MeetingFriends::onEnter() {
    stickmanX = -8.0f;
    sceneTimer = 0.0f;
    manualCameraControl = false;
    cameraDistance = 10.0f;
    cameraAngleY = 0.0f;
    cameraAngleX = 20.0f;
    cameraTargetX = 1.0f;
    cameraTargetY = 1.0f;
    cameraTargetZ = 0.0f;
    
    friends[0].idlePhase = 0.0f;
    friends[1].idlePhase = 0.5f;
    friends[2].idlePhase = 1.0f;
    friends[3].idlePhase = 1.5f;
}

void Scene3_MeetingFriends::update(float deltaTime) {
    sceneTimer += deltaTime;
    
    // Main character rides in fast, then slows down
    if (stickmanX < 0.0f) {
        float speed = 0.08f * (1.0f - stickmanX / -8.0f * 0.5f);  // Decelerate
        stickmanX += speed;
    }
    
    for (int i = 0; i < 4; i++) {
        friends[i].idlePhase += 0.02f;
    }
}

void Scene3_MeetingFriends::render() {
    drawParkEnvironment();
    
    // Main character riding in with speed effects
    glPushMatrix();
    glTranslatef(stickmanX, 0.0f, 0.0f);
    
    float pedalSpeed = fmaxf(0.1f, (0.0f - stickmanX) * 0.05f);
    
    if (stickmanX < -2.0f) {
        // Still coming in fast - speed lines!
        float speedIntensity = fminf(1.0f, (-stickmanX - 2.0f) * 0.3f);
        drawBike(0);  // Hot Pink - main character
        drawStickmanOnBike(sceneTimer * 5.0f, 0.9f, 0);
        drawSpeedLines(stickmanX - 0.3f, 0.6f, 0.0f, speedIntensity, 1.0f);
    } else {
        // Slowing down for the meet-up
        drawBike(0);  // Hot Pink - main character
        drawStickmanOnBike(sceneTimer * 2.0f * pedalSpeed, 0.9f, 0);
    }
    glPopMatrix();
    
    // Friends doing victory waves/fist pumps as main character arrives
    float helmetHues[4] = {0.3f, 0.6f, 1.0f, 0.15f};
    
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(friends[i].x, 0.0f, friends[i].z);
        glRotatef(friends[i].bikeRotation - 20.0f, 0.0f, 1.0f, 0.0f);  // Angled toward main char
        
        // Friends do excited idle animation
        float excitement = sinf(sceneTimer * 3.0f + friends[i].idlePhase) * 0.15f;
        drawBike(i + 1);  // Colors: 1=Blue, 2=Green, 3=Orange, 4=Purple
        
        // Victory pose when main character is close
        if (stickmanX > -2.0f && i == 0) {
            drawStickmanVictory(sceneTimer, helmetHues[i], i + 1);
        } else {
            drawStickmanOnBike(sceneTimer * 2.0f + friends[i].idlePhase, helmetHues[i], i + 1);
        }
        glPopMatrix();
    }
    
    // IMPACT BURST when they meet!
    if (stickmanX > -1.5f && stickmanX < 0.5f) {
        float burstPhase = (stickmanX + 1.5f) / 2.0f;
        if (burstPhase < 0.5f) {
            drawImpactBurst(1.0f, 1.5f, -1.0f, 1.0f + burstPhase, sceneTimer);
        }
    }
}

void Scene3_MeetingFriends::setupCamera() {
    float t = sceneTimer;
    
    if (t < 2.0f) {
        // DRAMATIC: Wide shot of friends waiting, main char incoming
        float panProgress = t / 2.0f;
        gluLookAt(8.0f - panProgress * 3.0f, 2.5f, 12.0f,
                 0.0f, 1.0f, 0.0f,
                 0.0, 1.0, 0.0);
    } else if (t < 4.0f) {
        // TRACKING: Follow main character riding in
        float camX = stickmanX + 4.0f;
        float shake = sinf(t * 15.0f) * 0.03f * fmaxf(0.0f, -stickmanX * 0.1f);
        gluLookAt(camX, 1.5f + shake, 6.0f,
                 stickmanX + 1.0f, 1.0f, 0.0f,
                 shake, 1.0, 0.0);
    } else if (t < 6.0f) {
        // CLOSE-UP: Reaction shots cycling between characters
        float cycleProgress = fmodf((t - 4.0f) * 2.0f, 4.0f);
        int focusChar = (int)cycleProgress;
        float targetX = (focusChar == 0) ? stickmanX : friends[focusChar - 1].x;
        float targetZ = (focusChar == 0) ? 0.0f : friends[focusChar - 1].z;
        
        gluLookAt(targetX + 1.5f, 1.8f, targetZ + 3.0f,
                 targetX, 1.5f, targetZ,
                 0.0, 1.0, 0.0);
    } else {
        // GROUP SHOT: Pull back to show everyone together
        float pullback = fminf(1.0f, (t - 6.0f) * 0.25f);
        gluLookAt(3.0f, 3.0f + pullback, 10.0f + pullback * 4.0f,
                 3.0f, 1.0f, -1.0f,
                 0.0, 1.0, 0.0);
    }
}

bool Scene3_MeetingFriends::isComplete() {
    return sceneTimer >= 10.0f;
}
