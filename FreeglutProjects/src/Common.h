#ifndef COMMON_H
#define COMMON_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>

const float TWO_PI = 2.0f * 3.1415926f;

// Scene enumeration
enum Scene {
    SCENE_1_BIKE_IN_GARAGE,
    SCENE_2_GARAGE_TAKEOFF,
    SCENE_3_MEETING_FRIENDS,
    SCENE_4_GROUP_BIKING,
    SCENE_5_FINAL_POSE
};

// Friend structure for Scene 3
struct Friend {
    float x, z;
    float idlePhase;
    float bikeRotation;
};

// Global camera variables
extern float cameraDistance;
extern float cameraAngleY;
extern float cameraAngleX;
extern float cameraTargetX;
extern float cameraTargetY;
extern float cameraTargetZ;
extern bool manualCameraControl;

// Lighting
extern GLfloat lightPos[];
extern GLfloat lightColor[];
extern GLfloat ambientLight[];
extern GLfloat specularLight[];
extern GLfloat matShininess;

#endif // COMMON_H
