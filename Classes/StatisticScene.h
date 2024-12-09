#pragma once

#include "cocos2d.h"
#include <vector>

class StatisticScene : public cocos2d::Scene
{
private:
	// Vector with the solutions 
	std::vector<int> solution;

	// Colors
	cocos2d::Color4B stuffedLetters;

public:
	// Create the scene
	StatisticScene(const std::vector<int>& a);
	virtual bool init() override;
	static StatisticScene* createScene(const std::vector<int>& a);

	// Callbacks
	void returnCallback(Ref* pSender);
	void exitCallback(Ref* pSender);

	// Set exit and return messages and background
	void setSorrounding();

	// Set text in a certain position
	void setStatistics(int sol, int gen, cocos2d::Vec2 pos);
};