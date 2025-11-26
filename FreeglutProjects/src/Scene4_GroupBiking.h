#ifndef SCENE4_GROUP_BIKING_H
#define SCENE4_GROUP_BIKING_H

#include "SceneBase.h"

class Scene4_GroupBiking : public SceneBase {
private:
    float groupBikeProgress;
    float groupBikeSpeed;
    float walkPhase;
    float sceneTimer;

public:
    Scene4_GroupBiking();
    
    void onEnter() override;
    void update(float deltaTime) override;
    void render() override;
    void setupCamera() override;
    bool isComplete() override;
    bool allowsManualCamera() override { return false; }
};

#endif // SCENE4_GROUP_BIKING_H
