#pragma once

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Scene
{
public:
    // Create a static createScene function for this scene
    static cocos2d::Scene* createScene();
    virtual bool init();

    // Callbacks for menu buttons
    void menuPlayCallback(cocos2d::Ref* pSender);
    void menuExitCallback(cocos2d::Ref* pSender);

    // Macro to implement the "create" method
    CREATE_FUNC(MainMenuScene);
};
