#pragma once

#include "cocos2d.h"
#include <vector>
#include "Particle.h"

class GameScene : public cocos2d::Scene 
{
private:
    int level;
    int iteration;
    int solved;
    int unscheduleCount;
    int numParticles;

    // Colors
    cocos2d::Color4B stuffedLetters;

    // Messages
    cocos2d::Label* generationLabel;
    cocos2d::Label* solvedLabel;

    // Vector of particles
    std::vector<Particle*> particles;

    // Initial position
    Vec2 initialPosition;

    // Solutions 
    std::vector<int> solutions;

public:
    GameScene(int level);  // Constructor que acepta el valor

    virtual bool init();
    static GameScene* createScene(int level);

    // Create room
    void generateRoom();

    // Crate walls
    void createVerticalStaticWall(cocos2d::Vec2 start, cocos2d::Vec2 end, float thickness, bool exit = false);
    void createHorizontalStaticWall(cocos2d::Vec2 start, cocos2d::Vec2 end, float thickness);
    void createRotatingWall(cocos2d::Vec2 start, cocos2d::Vec2 end, float thickness, bool mov);
    void createMovingWall(cocos2d::Vec2 start, cocos2d::Vec2 end, float thicness, float movement);
    
    // Create messages
    void generateMessages();
    void updateGeneration();
    void updateSolved();

    // Callbacks
    void returnCallback(Ref* pSender);
    void exitCallback(Ref* pSender);

    // Management of particles
    void handleUnscheduleParticle(Particle* particle);
    void restartParticles();
    void newGeneration(std::vector<Particle*>& nextGeneration);
    void mergeSolutions(std::vector<float>&, std::vector<float>&, const std::vector<float>&, const std::vector<float>&);
    
    // Shor results
    void showStatistics();
};
