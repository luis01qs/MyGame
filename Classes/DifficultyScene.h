#pragma once

#include "cocos2d.h"

class DifficultyScene : public cocos2d::Scene
{
private:
    int level_index;

public:
    // Create a static createScene function for this scene
    static cocos2d::Scene* createScene();
    virtual bool init();

    // Callbacks for menu buttons
    void easyCallback(cocos2d::Ref* pSender);
    void mediumCallback(cocos2d::Ref* pSender);
    void hardCallback(cocos2d::Ref* pSender);
    void returnCallback(cocos2d::Ref* pSender);

    // Macro to implement the "create" method
    CREATE_FUNC(DifficultyScene);
};
