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
            drawBike(i);  // Each rider has unique bike color
            drawStickmanVictory(sceneTimer + i * 0.3f, helmetHues[i], i);
        } else {
            // Quick skid-stop animation at start
            float skidProgress = sceneTimer;
            glRotatef(-skidProgress * 20.0f, 0, 0, 1);
            drawBike(i);  // Each rider has unique bike color
            drawStickmanOnBike(sceneTimer * 10.0f, helmetHues[i], i);
            
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
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // ========== IMPROVED LOGO BACKGROUND ==========
        
        // OUTER GLOW - Hot Pink
        float glowPulse = 1.0f + sinf(sceneTimer * 8.0f) * 0.15f;
        for (int g = 5; g >= 1; g--) {
            float glowSize = 0.02f * g * glowPulse;
            glColor4f(1.0f, 0.2f, 0.5f, fadeIn * 0.15f / g);
            glBegin(GL_POLYGON);
            glVertex2f(-0.65f - glowSize, -0.05f - glowSize);
            glVertex2f(0.65f + glowSize, -0.05f - glowSize);
            glVertex2f(0.65f + glowSize, 0.4f + glowSize);
            glVertex2f(-0.65f - glowSize, 0.4f + glowSize);
            glEnd();
        }
        
        // SOLID BANNER BACKGROUND - Dark with gradient feel
        // Main black background
        glColor4f(0.1f, 0.08f, 0.15f, fadeIn * 0.95f);
        glBegin(GL_POLYGON);
        glVertex2f(-0.62f, -0.02f);
        glVertex2f(0.62f, -0.02f);
        glVertex2f(0.62f, 0.38f);
        glVertex2f(-0.62f, 0.38f);
        glEnd();
        
        // Hot Pink border frame
        glColor4f(1.0f, 0.2f, 0.5f, fadeIn);
        glLineWidth(6.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(-0.62f, -0.02f);
        glVertex2f(0.62f, -0.02f);
        glVertex2f(0.62f, 0.38f);
        glVertex2f(-0.62f, 0.38f);
        glEnd();
        
        // Inner border - Electric Blue accent
        glColor4f(0.1f, 0.6f, 1.0f, fadeIn * 0.8f);
        glLineWidth(3.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(-0.58f, 0.02f);
        glVertex2f(0.58f, 0.02f);
        glVertex2f(0.58f, 0.34f);
        glVertex2f(-0.58f, 0.34f);
        glEnd();
        
        // Corner accents - small diamonds
        float cornerSize = 0.04f;
        float corners[4][2] = {{-0.62f, -0.02f}, {0.62f, -0.02f}, {0.62f, 0.38f}, {-0.62f, 0.38f}};
        for (int c = 0; c < 4; c++) {
            glColor4f(1.0f, 0.8f, 0.2f, fadeIn);  // Gold corners
            glBegin(GL_POLYGON);
            glVertex2f(corners[c][0], corners[c][1] + cornerSize);
            glVertex2f(corners[c][0] + cornerSize, corners[c][1]);
            glVertex2f(corners[c][0], corners[c][1] - cornerSize);
            glVertex2f(corners[c][0] - cornerSize, corners[c][1]);
            glEnd();
        }
        
        // Starburst effect behind text (rotating rays)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        int numRays = 16;
        for (int i = 0; i < numRays; i++) {
            float angle = i * 6.28318f / numRays + sceneTimer * 0.5f;
            float rayLen = 0.3f + (i % 2) * 0.15f;
            float rayR = (i % 3 == 0) ? 1.0f : 0.1f;
            float rayG = (i % 3 == 1) ? 0.6f : 0.2f;
            float rayB = (i % 3 == 2) ? 1.0f : 0.5f;
            glColor4f(rayR, rayG, rayB, fadeIn * 0.2f);
            glLineWidth(4.0f);
            glBegin(GL_LINES);
            glVertex2f(0.0f, 0.18f);
            glVertex2f(cosf(angle) * rayLen, 0.18f + sinf(angle) * rayLen * 0.4f);
            glEnd();
        }
        
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Main title: "KIK BATWOWZKI"
        // Draw shadow/outline first
        glColor4f(0.0f, 0.0f, 0.0f, fadeIn);
        glLineWidth(8.0f);
        glPushMatrix();
        glTranslatef(-0.52f, 0.10f, 0.0f);
        glScalef(0.0007f * scale, 0.0012f * scale, 1.0f);
        const char* title = "KIK BATWOWZKI";
        for (const char* c = title; *c; c++) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        // Main text - HOT PINK
        glColor4f(1.0f, 0.2f, 0.55f, fadeIn);
        glLineWidth(5.0f);
        glPushMatrix();
        glTranslatef(-0.5f, 0.12f, 0.0f);
        glScalef(0.0007f * scale, 0.0012f * scale, 1.0f);
        for (const char* c = title; *c; c++) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        // Inner highlight - white shine
        glColor4f(1.0f, 1.0f, 1.0f, fadeIn * 0.6f);
        glLineWidth(2.5f);
        glPushMatrix();
        glTranslatef(-0.49f, 0.14f, 0.0f);
        glScalef(0.00068f * scale, 0.00115f * scale, 1.0f);
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
