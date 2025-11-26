#ifndef SCENE5_FINAL_POSE_H
#define SCENE5_FINAL_POSE_H

#include "SceneBase.h"

class Scene5_FinalPose : public SceneBase {
private:
    float sceneTimer;

public:
    Scene5_FinalPose();
    
    void onEnter() override;
    void update(float deltaTime) override;
    void render() override;
    void setupCamera() override;
    bool isComplete() override;
    bool allowsManualCamera() override { return false; }
};

#endif // SCENE5_FINAL_POSE_H
