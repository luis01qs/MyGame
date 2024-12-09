#pragma once

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class Particle : public cocos2d::Node
{
public:
	// Sprite and physical bode
	Sprite* sprite;
	PhysicsBody* physicsBody;

	// Current properties of the Particle
	Vec2 position;
	Vec2 velocity;
	float distance;
	float speed;
	float angle;
	float exit;

	// Genetic information
	std::vector<float> deltaAngle;
	int vectorSize;
	int currentIndex;
	int currentIteration;

	// Creation of the object
	Particle(Vec2 position);
	virtual bool init() override;

	// Function that is called to update every frame
	virtual void update(float deltaTime) override;

	// Create function
	static Particle* create(cocos2d::Vec2 position);

	// Callback for unschedule
	std::function<void(Particle*)> onUnscheduleCallback;

	// Operator <
	bool operator<(const Particle& other) const {
		return distance < other.distance;
	}

	// Start the movement
	void start();

	// Mutation
	void mutation();
};