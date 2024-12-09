#pragma once

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class StatisticScene : public cocos2d::Scene
{
private:
	std::vector<int> solutions;
public:
	StatisticScene(const std::vector<int>& a);

	virtual bool init() override;

	static StatisticScene* createScene(const std::vector<int>& a);
};