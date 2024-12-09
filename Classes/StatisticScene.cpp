#include "StatisticScene.h"
#include "DifficultyScene.h"
#include <vector>

USING_NS_CC;

StatisticScene::StatisticScene(const std::vector<int> &a)
{
	this->solution = a;
}

StatisticScene* StatisticScene::createScene(const std::vector<int>& a)
{
	StatisticScene* scene = new StatisticScene(a);
	if (scene && scene->init())
	{
		scene->autorelease();
		return scene;
	}
	else
	{
		delete scene;
		return nullptr;
	}
}

bool StatisticScene::init()
{
	if (!Scene::init()) return false;

	// Set exit, return and background
	setSorrounding();

    // Get the visible size of the window and its origin
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    for (int i = 0; i < 5; ++i)
    {
        setStatistics(solution[i], i + 1, Vec2(450, 300 + 850 - 200 * i));
    }

    for (int i = 5; i < 10; ++i)
    {
        setStatistics(solution[i], i + 1, Vec2(150 + visibleSize.width / 2, 300 + 850 - 200 * (i - 5)));
    }

	return true;
}

// Exit, return and background
void StatisticScene::setSorrounding()
{
    // Set color for the letters
    stuffedLetters = Color4B(200, 200, 200, 255);

    // Get the visible size of the window and its origin
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Background
    auto background = LayerColor::create(Color4B(77, 94, 120, 100));
    float scaleX = visibleSize.width / background->getContentSize().width;
    float scaleY = visibleSize.height / background->getContentSize().height;
    background->setScale(scaleX, scaleY);
    this->addChild(background, -1);

    // Return
    auto returnButton = Label::createWithTTF("Return", "fonts/Regular.ttf", 80);
    returnButton->setTextColor(stuffedLetters);
    auto returnItem = MenuItemLabel::create(returnButton, CC_CALLBACK_1(StatisticScene::returnCallback, this));
    returnItem->setAnchorPoint(Vec2(0, 1));
    returnItem->setPosition(Vec2(origin.x + 80, origin.y + visibleSize.height - 60));

    // Exit button
    auto exitButton = Label::createWithTTF("Exit", "fonts/Regular.ttf", 80);
    exitButton->setTextColor(stuffedLetters);
    auto exitItem = MenuItemLabel::create(exitButton, CC_CALLBACK_1(StatisticScene::exitCallback, this));
    exitItem->setAnchorPoint(Vec2(1, 1));
    exitItem->setPosition(Vec2(origin.x + visibleSize.width - 80, origin.y + visibleSize.height - 60));

    auto menu = Menu::create(returnItem, exitItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return;
}

// Callbacks

void StatisticScene::returnCallback(Ref* pSender)
{
    auto menuScene = DifficultyScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, menuScene));
}

void StatisticScene::exitCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

// Set text

void StatisticScene::setStatistics(int sol, int gen, Vec2 pos)
{
    std::string labelText = "Generation " + std::to_string(gen) + ": " + std::to_string(sol);
    auto label = Label::createWithTTF(labelText, "fonts/Regular.ttf", 80);
    label->setTextColor(stuffedLetters);
    label->setAnchorPoint(Vec2(0, 1));
    label->setPosition(pos);
    this->addChild(label);

    return;
}