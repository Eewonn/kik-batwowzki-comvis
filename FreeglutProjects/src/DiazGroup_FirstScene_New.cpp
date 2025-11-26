// Main coordinator for multi-scene bike animation
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "AudioManager.h"
#include "Common.h"
#include "SceneBase.h"
#include "Scene1_BikeInGarage.h"
#include "Scene2_GarageTakeoff.h"
#include "Scene3_MeetingFriends.h"
#include "Scene4_GroupBiking.h"
#include "Scene5_FinalPose.h"

// Camera control variables (extern in Common.h)
float cameraDistance = 8.0f;
float cameraAngleY = 0.0f;
float cameraAngleX = 20.0f;
float cameraTargetX = 0.0f;
float cameraTargetY = 1.0f;
float cameraTargetZ = 0.0f;
bool manualCameraControl = true;

// Lighting (extern in Common.h)
GLfloat lightPos[] = { 3.0f, 4.0f, 2.0f, 1.0f };
GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat ambientLight[] = { 0.3f, 0.3f, 0.4f, 1.0f };
GLfloat specularLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat matShininess = 32.0f;

// Scene management
Scene currentScene = SCENE_1_BIKE_IN_GARAGE;
SceneBase* scenes[5];
SceneBase* activeScene = nullptr;

// Audio
AudioManager* audioManager = nullptr;
int backgroundMusicIndex = -1;

void switchToScene(Scene newScene) {
    currentScene = newScene;
    activeScene = scenes[newScene];
    activeScene->onEnter();
    
    const char* sceneNames[] = {
        "Scene 1: Bike in Garage (Manual Camera)",
        "Scene 2: Garage Take Off (Auto Camera)",
        "Scene 3: Meeting Friends (Manual Camera)",
        "Scene 4: Group Biking (Auto Camera)",
        "Scene 5: Final Pose (Auto Camera)"
    };
    std::cout << sceneNames[newScene] << std::endl;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Setup camera
    activeScene->setupCamera();

    // Enhanced lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    
    glEnable(GL_LIGHT1);
    GLfloat light1Pos[] = { -2.0f, 3.0f, 4.0f, 1.0f };
    GLfloat light1Color[] = { 0.6f, 0.7f, 0.8f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Color);

    // Render active scene
    activeScene->render();
    
    glutSwapBuffers();
}

void update(int v) {
    activeScene->update(0.016f);
    
    // Auto-advance all scenes seamlessly
    if (activeScene->isComplete()) {
        switch (currentScene) {
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
            default:
                break;
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case ' ': // Space to skip to next scene
            if (currentScene < SCENE_5_FINAL_POSE) {
                switchToScene((Scene)(currentScene + 1));
            }
            break;
            
        case 'r': // R to restart from beginning
        case 'R':
            switchToScene(SCENE_1_BIKE_IN_GARAGE);
            break;
            
        case 27: // ESC to quit
            if (audioManager) {
                delete audioManager;
            }
            exit(0);
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
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    if (!activeScene->allowsManualCamera()) return;
    
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
    if (!activeScene->allowsManualCamera()) return;
    
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
    
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 900.0 / 600.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
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
    
    backgroundMusicIndex = audioManager->loadSound("../sounds/background.wav");
    
    if (backgroundMusicIndex >= 0) {
        audioManager->setVolume(backgroundMusicIndex, 0.4f);
        audioManager->playSound(backgroundMusicIndex, true);
    } else {
        std::cerr << "Failed to load background music" << std::endl;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 600);
    glutCreateWindow("Bike Animation - Auto-Play | SPACE to skip | R to restart | 1-5 for scenes");

    glewInit();
    initGL();
    initAudio();

    // Initialize all scenes
    scenes[0] = new Scene1_BikeInGarage();
    scenes[1] = new Scene2_GarageTakeoff();
    scenes[2] = new Scene3_MeetingFriends();
    scenes[3] = new Scene4_GroupBiking();
    scenes[4] = new Scene5_FinalPose();
    
    // Start with scene 1
    switchToScene(SCENE_1_BIKE_IN_GARAGE);

    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseWheelFunc(mouseWheel);
    
    std::cout << "=== Controls ===" << std::endl;
    std::cout << "Animation auto-plays through all scenes!" << std::endl;
    std::cout << "SPACE: Skip to next scene" << std::endl;
    std::cout << "R: Restart from beginning" << std::endl;
    std::cout << "1-5: Jump to specific scene" << std::endl;
    std::cout << "ESC: Exit" << std::endl;
    std::cout << "===============" << std::endl;
    
    atexit([]() {
        if (audioManager) {
            delete audioManager;
            audioManager = nullptr;
        }
        for (int i = 0; i < 5; i++) {
            delete scenes[i];
        }
    });
    
    glutMainLoop();
    return 0;
}
