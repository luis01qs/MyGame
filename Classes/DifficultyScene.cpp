#include "DifficultyScene.h"
#include "cocos2d.h"
#include "AudioEngine.h"
#include "GameScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* DifficultyScene::createScene()
{
    return DifficultyScene::create();
}

bool DifficultyScene::init()
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

    // Font color
    auto stuffedLetters = Color4B(200, 200, 200, 255);

    // Easy button
    auto easyButton = Label::createWithTTF("Easy", "fonts/Regular.ttf", 140);
    easyButton->setTextColor(stuffedLetters);
    auto easyItem = MenuItemLabel::create(easyButton, CC_CALLBACK_1(DifficultyScene::easyCallback, this));
    easyItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 300));

    // Medium button
    auto mediumButton = Label::createWithTTF("Medium", "fonts/Regular.ttf", 140);
    mediumButton->setTextColor(stuffedLetters);
    auto mediumItem = MenuItemLabel::create(mediumButton, CC_CALLBACK_1(DifficultyScene::mediumCallback, this));
    mediumItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

    // Hard button
    auto hardButton = Label::createWithTTF("Hard", "fonts/Regular.ttf", 140);
    hardButton->setTextColor(stuffedLetters);
    auto hardItem = MenuItemLabel::create(hardButton, CC_CALLBACK_1(DifficultyScene::hardCallback, this));
    hardItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 300));

    // Return button
    auto returnButton = Label::createWithTTF("Return", "fonts/Regular.ttf", 80);
    returnButton->setTextColor(stuffedLetters);
    auto returnItem = MenuItemLabel::create(returnButton, CC_CALLBACK_1(DifficultyScene::returnCallback, this));
    returnItem->setAnchorPoint(Vec2(0, 1));
    returnItem->setPosition(Vec2(origin.x + 80, origin.y + visibleSize.height - 60));

    auto menu = Menu::create(easyItem, mediumItem, hardItem, returnItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}

void DifficultyScene::easyCallback(Ref* pSender)
{
    int level = 1;
    auto gameScene = new GameScene(level);
    gameScene->init();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameScene));
}

void DifficultyScene::mediumCallback(Ref* pSender)
{
    int level = 2;
    auto gameScene = new GameScene(level);
    gameScene->init();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameScene));
}

void DifficultyScene::hardCallback(Ref* pSender)
{
    int level = 3;
    auto gameScene = new GameScene(level);
    gameScene->init();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameScene));
}

void DifficultyScene::returnCallback(Ref* pSender)
{
    auto menuScene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, menuScene));
}
