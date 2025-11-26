#ifndef SCENE2_GARAGE_TAKEOFF_H
#define SCENE2_GARAGE_TAKEOFF_H

#include "SceneBase.h"

class Scene2_GarageTakeoff : public SceneBase {
private:
    float stickmanX;
    float bikeX;
    float walkSpeed;
    float walkPhase;
    float pedalPhase;
    bool walkingStarted;
    bool isMounted;
    bool doorOpening;
    bool doorFullyOpen;
    float doorProgress;
    float sceneTimer;

public:
    Scene2_GarageTakeoff();
    
    void onEnter() override;
    void update(float deltaTime) override;
    void render() override;
    void setupCamera() override;
    bool isComplete() override;
    bool allowsManualCamera() override { return false; }
};

#endif // SCENE2_GARAGE_TAKEOFF_H
