#include "MainMenuScene.h"
#include "cocos2d.h"
#include "AudioEngine.h"
#include "DifficultyScene.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    return MainMenuScene::create();
}

bool MainMenuScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // Get the visible size of the window and its origin
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Background
    auto background = LayerColor::create(Color4B(77, 94, 120, 100));
    float scaleX = visibleSize.width / background->getContentSize().width;
    float scaleY = visibleSize.height / background->getContentSize().height;
    background->setScale(scaleX, scaleY);
    this->addChild(background, -1);

    // Menu information
    
    // Font color
    auto stuffedLetters = Color4B(200, 200, 200, 255);

    // Play button
    auto playButton = Label::createWithTTF("Play", "fonts/Regular.ttf", 200);
    playButton->setTextColor(stuffedLetters);
    auto playItem = MenuItemLabel::create(playButton, CC_CALLBACK_1(MainMenuScene::menuPlayCallback, this));
    playItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 200));

    // Exit button
    auto exitButton = Label::createWithTTF("Exit", "fonts/Regular.ttf", 200);
    exitButton->setTextColor(stuffedLetters);
    auto exitItem = MenuItemLabel::create(exitButton, CC_CALLBACK_1(MainMenuScene::menuExitCallback, this));
    exitItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 200));

    //Create menu
    auto menu = Menu::create(playItem, exitItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}

// Buttons
void MainMenuScene::menuPlayCallback(Ref* pSender)
{
    auto difficutlyScene = DifficultyScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, difficutlyScene));
}

void MainMenuScene::menuExitCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

