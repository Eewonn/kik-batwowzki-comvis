#include "Scene5_FinalPose.h"

Scene5_FinalPose::Scene5_FinalPose() : sceneTimer(0.0f) {
}

void Scene5_FinalPose::onEnter() {
    sceneTimer = 0.0f;
    manualCameraControl = false;
}

void Scene5_FinalPose::update(float deltaTime) {
    sceneTimer += deltaTime;
}

void Scene5_FinalPose::render() {
    drawParkEnvironment();
    
    // KICK BUTTOWSKI FINALE: Epic group pose with victory effects!
    
    // V-Formation - victory stance
    float formations[5][2] = {
        {0.0f, 0.0f},      // Leader (main character) - center front
        {-1.5f, -1.0f},    // Left wing
        {1.5f, -1.0f},     // Right wing  
        {-2.5f, -2.0f},    // Far left
        {2.5f, -2.0f}      // Far right
    };
    
    float helmetHues[5] = {0.9f, 0.3f, 0.6f, 1.0f, 0.15f};
    
    for (int i = 0; i < 5; i++) {
        float xPos = formations[i][0] + 2.0f;
        float zPos = formations[i][1];
        
        // Triumphant bounce animation
        float bounce = fabsf(sinf(sceneTimer * 3.0f + i * 0.4f)) * 0.08f;
        
        glPushMatrix();
        glTranslatef(xPos, bounce, zPos);
        
        // All doing victory poses!
        if (sceneTimer > 1.0f) {
            drawBike();
            drawStickmanVictory(sceneTimer + i * 0.3f, helmetHues[i]);
        } else {
            // Quick skid-stop animation at start
            float skidProgress = sceneTimer;
            glRotatef(-skidProgress * 20.0f, 0, 0, 1);
            drawBike();
            drawStickmanOnBike(sceneTimer * 10.0f, helmetHues[i]);
            
            // Dust from skid
            if (i == 0) {
                drawDustCloud(xPos - 0.5f, 0.1f, zPos, 0.5f, sceneTimer);
            }
        }
        glPopMatrix();
    }
    
    // EPIC IMPACT BURST behind the group!
    if (sceneTimer > 0.5f && sceneTimer < 3.0f) {
        float burstPhase = (sceneTimer - 0.5f) / 2.5f;
        float burstSize = 2.0f * sinf(burstPhase * 3.14159f);
        if (burstSize > 0.3f) {
            drawImpactBurst(2.0f, 2.0f, -3.0f, burstSize, sceneTimer);
        }
    }
    
    // Radial zoom lines for that freeze-frame finale feel
    if (sceneTimer > 1.5f) {
        float zoomIntensity = fminf(0.4f, (sceneTimer - 1.5f) * 0.15f);
        float pulse = 1.0f + sinf(sceneTimer * 4.0f) * 0.2f;
        drawZoomLines(0, 0, zoomIntensity * pulse);
    }
    
    // "KIK BATWOWZKI" LOGO - appears near the end!
    if (sceneTimer > 5.0f) {
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        
        // Switch to 2D orthographic for text overlay
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(-1, 1, -1, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        float fadeIn = fminf(1.0f, (sceneTimer - 5.0f) * 2.0f);
        float pulse = 1.0f + sinf(sceneTimer * 6.0f) * 0.05f;
        float scale = fadeIn * pulse;
        
        // Draw bold background burst behind text
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        
        // Starburst effect behind logo
        int numRays = 24;
        for (int i = 0; i < numRays; i++) {
            float angle = i * 6.28318f / numRays + sceneTimer * 0.3f;
            float rayLen = 0.8f + (i % 2) * 0.3f;
            glColor4f(1.0f, 0.8f, 0.3f, fadeIn * 0.4f);
            glLineWidth(3.0f);
            glBegin(GL_LINES);
            glVertex2f(0.0f, 0.15f);
            glVertex2f(cosf(angle) * rayLen, 0.15f + sinf(angle) * rayLen * 0.5f);
            glEnd();
        }
        
        // Main title: "KIK BATWOWZKI"
        // Draw shadow/outline first
        glColor4f(0.0f, 0.0f, 0.0f, fadeIn);
        glLineWidth(6.0f);
        glPushMatrix();
        glTranslatef(-0.52f, 0.12f, 0.0f);
        glScalef(0.0007f * scale, 0.001f * scale, 1.0f);
        const char* title = "KIK BATWOWZKI";
        for (const char* c = title; *c; c++) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        // Main text - hot pink/magenta gradient
        glColor4f(1.0f, 0.3f, 0.6f, fadeIn);
        glLineWidth(4.0f);
        glPushMatrix();
        glTranslatef(-0.5f, 0.15f, 0.0f);
        glScalef(0.0007f * scale, 0.001f * scale, 1.0f);
        for (const char* c = title; *c; c++) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        // Inner highlight - white
        glColor4f(1.0f, 1.0f, 1.0f, fadeIn * 0.7f);
        glLineWidth(2.0f);
        glPushMatrix();
        glTranslatef(-0.49f, 0.16f, 0.0f);
        glScalef(0.00068f * scale, 0.00098f * scale, 1.0f);
        for (const char* c = title; *c; c++) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        glDisable(GL_BLEND);
        
        // Restore matrices
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
    }
}

void Scene5_FinalPose::setupCamera() {
    float t = sceneTimer;
    
    if (t < 1.0f) {
        // DRAMATIC: Low angle as they skid to a stop
        float progress = t;
        float camX = 2.0f + (1.0f - progress) * 8.0f;
        float shake = sinf(t * 30.0f) * 0.1f * (1.0f - progress);
        gluLookAt(camX, 0.5f + shake, 6.0f,
                 2.0f, 1.0f, 0.0f,
                 shake * 0.5f, 1.0, 0.0);
    } else if (t < 3.0f) {
        // HERO SHOT: Low angle looking up at victory poses
        float rise = (t - 1.0f) / 2.0f;
        gluLookAt(2.0f, 0.3f + rise * 0.3f, 7.0f - rise * 2.0f,
                 2.0f, 1.8f, -0.5f,
                 0.0, 1.0, 0.0);
    } else if (t < 5.0f) {
        // ORBIT: Slow dramatic orbit around the group
        float orbit = (t - 3.0f) * 0.8f;
        float camX = 2.0f + sinf(orbit) * 6.0f;
        float camZ = cosf(orbit) * 6.0f;
        gluLookAt(camX, 2.5f, camZ,
                 2.0f, 1.5f, -0.5f,
                 0.0, 1.0, 0.0);
    } else {
        // FREEZE FRAME: Classic Kick Buttowski ending - static hero shot
        float tilt = sinf(t * 0.5f) * 0.05f;  // Subtle movement
        gluLookAt(2.0f, 1.8f, 8.0f,
                 2.0f, 1.5f, 0.0f,
                 tilt, 1.0, 0.0);
    }
}

bool Scene5_FinalPose::isComplete() {
    return false; // End scene - loops or holds
}
