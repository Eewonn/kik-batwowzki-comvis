#ifndef SCENE1_BIKE_IN_GARAGE_H
#define SCENE1_BIKE_IN_GARAGE_H

#include "SceneBase.h"

class Scene1_BikeInGarage : public SceneBase {
private:
    float stickmanX;
    float bikeX;
    float sceneTimer;
    float walkPhase;
    float pedalPhase;
    bool isMounted;
    bool isRidingOut;
    float doorProgress;

public:
    Scene1_BikeInGarage();
    
    void onEnter() override;
    void update(float deltaTime) override;
    void render() override;
    void setupCamera() override;
    bool isComplete() override;
    bool allowsManualCamera() override { return false; }
};

#endif // SCENE1_BIKE_IN_GARAGE_H
