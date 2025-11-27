// Main entry point for Kik Batwowzki Animation
// Kick Buttowski inspired bike adventure!

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

#include "Common.h"
#include "SceneBase.h"
#include "Scene1_BikeInGarage.h"
#include "Scene2_GarageTakeoff.h"
#include "Scene3_MeetingFriends.h"
#include "Scene4_GroupBiking.h"
#include "Scene5_FinalPose.h"
#include "AudioManager.h"

// Global camera variables (defined here, extern in Common.h)
float cameraDistance = 8.0f;
float cameraAngleY = 0.0f;
float cameraAngleX = 20.0f;
float cameraTargetX = 0.0f;
float cameraTargetY = 1.0f;
float cameraTargetZ = 0.0f;
bool manualCameraControl = false;

// Lighting (defined here)
GLfloat lightPos[] = { 5.0f, 10.0f, 5.0f, 1.0f };
GLfloat lightColor[] = { 1.0f, 0.95f, 0.9f, 1.0f };
GLfloat ambientLight[] = { 0.3f, 0.3f, 0.35f, 1.0f };
GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat matShininess = 50.0f;

// Window dimensions
int windowWidth = 1280;
int windowHeight = 720;

// Scene management
SceneBase* currentScene = nullptr;
Scene currentSceneType = SCENE_1_BIKE_IN_GARAGE;

Scene1_BikeInGarage scene1;
Scene2_GarageTakeoff scene2;
Scene3_MeetingFriends scene3;
Scene4_GroupBiking scene4;
Scene5_FinalPose scene5;

// Audio manager
AudioManager audioManager;

// Timing
float lastTime = 0.0f;

void switchToScene(Scene newScene) {
    currentSceneType = newScene;
    
    switch (newScene) {
        case SCENE_1_BIKE_IN_GARAGE:
            currentScene = &scene1;
            break;
        case SCENE_2_GARAGE_TAKEOFF:
            currentScene = &scene2;
            break;
        case SCENE_3_MEETING_FRIENDS:
            currentScene = &scene3;
            break;
        case SCENE_4_GROUP_BIKING:
            currentScene = &scene4;
            break;
        case SCENE_5_FINAL_POSE:
            currentScene = &scene5;
            break;
    }
    
    currentScene->onEnter();
}

void nextScene() {
    switch (currentSceneType) {
        case SCENE_1_BIKE_IN_GARAGE:
            switchToScene(SCENE_2_GARAGE_TAKEOFF);
            break;
        case SCENE_2_GARAGE_TAKEOFF:
            switchToScene(SCENE_3_MEETING_FRIENDS);
            break;
        case SCENE_3_MEETING_FRIENDS:
            switchToScene(SCENE_4_GROUP_BIKING);
            break;
        case SCENE_4_GROUP_BIKING:
            switchToScene(SCENE_5_FINAL_POSE);
            break;
        case SCENE_5_FINAL_POSE:
            // Loop back to start or stay at end
            switchToScene(SCENE_1_BIKE_IN_GARAGE);
            break;
    }
}

void initOpenGL() {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);
    
    // Enable anti-aliasing
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    // Set up lighting
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    
    // Background color - nice sky blue
    glClearColor(0.4f, 0.6f, 0.9f, 1.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)windowWidth / (float)windowHeight;
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
    
    // Set up modelview and camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Update lighting position
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    if (currentScene) {
        // Use manual camera or automatic scene camera
        if (manualCameraControl) {
            // Manual camera - orbit around target
            float radY = cameraAngleY * 3.14159f / 180.0f;
            float radX = cameraAngleX * 3.14159f / 180.0f;
            float camX = cameraTargetX + cameraDistance * cosf(radX) * sinf(radY);
            float camY = cameraTargetY + cameraDistance * sinf(radX);
            float camZ = cameraTargetZ + cameraDistance * cosf(radX) * cosf(radY);
            gluLookAt(camX, camY, camZ,
                     cameraTargetX, cameraTargetY, cameraTargetZ,
                     0.0, 1.0, 0.0);
        } else {
            // Automatic cinematic camera
            currentScene->setupCamera();
        }
        currentScene->render();
    }
    
    glutSwapBuffers();
}

void update(int value) {
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    
    if (currentScene) {
        currentScene->update(deltaTime);
        
        // Auto-advance scenes
        if (currentScene->isComplete()) {
            nextScene();
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);  // ~60 FPS
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:  // ESC
            exit(0);
            break;
        case ' ':  // Space - skip to next scene
            nextScene();
            break;
        case '1':
            switchToScene(SCENE_1_BIKE_IN_GARAGE);
            break;
        case '2':
            switchToScene(SCENE_2_GARAGE_TAKEOFF);
            break;
        case '3':
            switchToScene(SCENE_3_MEETING_FRIENDS);
            break;
        case '4':
            switchToScene(SCENE_4_GROUP_BIKING);
            break;
        case '5':
            switchToScene(SCENE_5_FINAL_POSE);
            break;
        case 'm':
        case 'M':
            manualCameraControl = !manualCameraControl;
            std::cout << "Manual camera: " << (manualCameraControl ? "ON" : "OFF") << std::endl;
            break;
        case 'r':
        case 'R':
            // Reset to scene 1
            switchToScene(SCENE_1_BIKE_IN_GARAGE);
            break;
    }
}

void specialKeys(int key, int x, int y) {
    if (manualCameraControl) {
        float rotSpeed = 5.0f;
        float moveSpeed = 0.5f;
        
        switch (key) {
            case GLUT_KEY_LEFT:
                cameraAngleY -= rotSpeed;
                break;
            case GLUT_KEY_RIGHT:
                cameraAngleY += rotSpeed;
                break;
            case GLUT_KEY_UP:
                cameraAngleX -= rotSpeed;
                if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
                break;
            case GLUT_KEY_DOWN:
                cameraAngleX += rotSpeed;
                if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
                break;
            case GLUT_KEY_PAGE_UP:
                cameraDistance -= moveSpeed;
                if (cameraDistance < 1.0f) cameraDistance = 1.0f;
                break;
            case GLUT_KEY_PAGE_DOWN:
                cameraDistance += moveSpeed;
                if (cameraDistance > 50.0f) cameraDistance = 50.0f;
                break;
        }
    }
}

int main(int argc, char** argv) {
    std::cout << "=== KIK BATWOWZKI ===" << std::endl;
    std::cout << "Kick Buttowski Style Bike Adventure!" << std::endl;
    std::cout << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  SPACE - Skip to next scene" << std::endl;
    std::cout << "  1-5   - Jump to specific scene" << std::endl;
    std::cout << "  M     - Toggle manual camera" << std::endl;
    std::cout << "  R     - Reset to Scene 1" << std::endl;
    std::cout << "  ESC   - Exit" << std::endl;
    std::cout << std::endl;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("KIK BATWOWZKI - Bike Adventure!");
    
    initOpenGL();
    
    // Initialize audio and load background music
    if (audioManager.initialize()) {
        int bgMusic = audioManager.loadSound("../sounds/background.wav");
        if (bgMusic >= 0) {
            audioManager.setVolume(bgMusic, 0.5f);  // 50% volume
            audioManager.playSound(bgMusic, true);  // Loop the background music
            std::cout << "Background music loaded and playing!" << std::endl;
        } else {
            std::cout << "Warning: Could not load background music" << std::endl;
        }
    } else {
        std::cout << "Warning: Audio system failed to initialize" << std::endl;
    }
    
    // Start with Scene 1
    switchToScene(SCENE_1_BIKE_IN_GARAGE);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, update, 0);
    
    lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    
    glutMainLoop();
    
    return 0;
}
