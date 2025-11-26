#ifndef SCENE3_MEETING_FRIENDS_H
#define SCENE3_MEETING_FRIENDS_H

#include "SceneBase.h"

class Scene3_MeetingFriends : public SceneBase {
private:
    float stickmanX;
    Friend friends[4];
    float sceneTimer;

public:
    Scene3_MeetingFriends();
    
    void onEnter() override;
    void update(float deltaTime) override;
    void render() override;
    void setupCamera() override;
    bool isComplete() override;
    bool allowsManualCamera() override { return false; }
};

#endif // SCENE3_MEETING_FRIENDS_H
