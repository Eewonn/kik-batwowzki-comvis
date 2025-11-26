#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Common.h"
#include "DrawingUtils.h"

class SceneBase {
public:
    virtual ~SceneBase() {}
    
    // Called when scene starts
    virtual void onEnter() = 0;
    
    // Called every frame to update scene logic
    virtual void update(float deltaTime) = 0;
    
    // Called every frame to render scene
    virtual void render() = 0;
    
    // Called to setup camera for this scene
    virtual void setupCamera() = 0;
    
    // Check if scene is complete and should advance
    virtual bool isComplete() = 0;
    
    // Is manual camera control allowed?
    virtual bool allowsManualCamera() = 0;
};

#endif // SCENE_BASE_H
