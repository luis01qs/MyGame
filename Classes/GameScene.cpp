#include "GameScene.h"
#include "DifficultyScene.h"
#include "StatisticScene.h"
#include "audio/include/AudioEngine.h"
#include "Walls.h"
#include "Particle.h"
#include <cstdlib>
#include <ctime>

USING_NS_CC;

GameScene::GameScene(int level)
{
    this->level = level;  // Guardamos el valor recibido
}

GameScene* GameScene::createScene(int level)
{
    GameScene* ret = new GameScene(level);  // Crear la escena usando el constructor personalizado
    if (ret && ret->init())
    {
        ret->autorelease();  // No olvides hacer autorelease para liberar la memoria automáticamente
        return ret;
    }
    else
    {
        delete ret;
        return nullptr;
    }
}

bool GameScene::init() 
{
    if (!Scene::initWithPhysics()) 
    {
        return false;
    }

    // Initialize variables
    iteration = 0;
    solved = 0;
    numParticles = 100;
    unscheduleCount = 0;
    stuffedLetters = Color4B(200, 200, 200, 255);
    std::srand(std::time(nullptr));

    // Create the room
    generateRoom();

    // Create messages
    generateMessages();

    //this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    this->getPhysicsWorld()->setGravity(Vec2(0, 0));
    this->getPhysicsWorld()->setFixedUpdateRate(1.0f / 240.0f); // Increase update rate for physics
    this->getPhysicsWorld()->setSubsteps(3);

    // Launch first generation
    restartParticles();
    
    return true;
}

// Room generation

void GameScene::generateRoom()
{    
    // Get the visible size of the window and its origin
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Background
    auto background = LayerColor::create(Color4B(77, 94, 120, 100));
    float scaleX = visibleSize.width / background->getContentSize().width;
    float scaleY = visibleSize.height / background->getContentSize().height;
    background->setScale(scaleX, scaleY);
    this->addChild(background, -1);

    // Set constants
    float thickness = 20.0f;
    float margin = 120;

    // Left wall
    createVerticalStaticWall(
        Vec2(origin.x + margin, origin.y + 2 * visibleSize.height / 3),
        Vec2(origin.x + margin, origin.y + visibleSize.height / 3),
        thickness
    );

    // Right wall
    createVerticalStaticWall(
        Vec2(origin.x + visibleSize.width - margin, origin.y + 2 * visibleSize.height / 3),
        Vec2(origin.x + visibleSize.width - margin, origin.y + visibleSize.height / 3),
        thickness, true
    );

    // Ceil
    createHorizontalStaticWall(
        Vec2(origin.x + margin - thickness / 2, origin.y + 2 * visibleSize.height / 3),
        Vec2(origin.x + visibleSize.width - margin + thickness / 2, origin.y + 2 * visibleSize.height / 3),
        thickness
    );

    // Floor
    createHorizontalStaticWall(
        Vec2(origin.x + margin - thickness / 2, origin.y + visibleSize.height / 3),
        Vec2(origin.x + visibleSize.width - margin + thickness / 2, origin.y + visibleSize.height / 3),
        thickness
    );

    // Maze
    int maze[5] = {1, 2, 3, 1, 2};
    if (level > 1) maze[4] = 4;
    if (level > 2) maze[3] = 5;

    Vec2 initialPosition = Vec2(origin.x + margin, origin.y + visibleSize.height / 2);
    for (int i = 0; i < 5; ++i) std::swap(maze[i], maze[rand() % 5]);
    for (int i = 0; i < 5; ++i)
    {
        switch (maze[i])
        {
        case 1:
            createVerticalStaticWall(
                Vec2(initialPosition.x + (i + 1) * 320, initialPosition.y - 100),
                Vec2(initialPosition.x + (i + 1) * 320, initialPosition.y + 100),
                thickness
            );
            break;

        case 2:
            createVerticalStaticWall(
                Vec2(initialPosition.x + (i + 1) * 320, initialPosition.y - 100),
                Vec2(initialPosition.x + (i + 1) * 320, initialPosition.y - 230),
                thickness
            );
            createVerticalStaticWall(
                Vec2(initialPosition.x + (i + 1) * 320, initialPosition.y + 100),
                Vec2(initialPosition.x + (i + 1) * 320, initialPosition.y + 230),
                thickness
            );
            break;

        case 3:
            createHorizontalStaticWall(
                Vec2(initialPosition.x + (i + 1) * 320 - 320 / 3, initialPosition.y),
                Vec2(initialPosition.x + (i + 1) * 320 + 320 / 3, initialPosition.y),
                thickness
            );
            break;

        case 4:
            createRotatingWall(
                Vec2(initialPosition.x + (i + 1) * 320, initialPosition.y + 90 - 60),
                Vec2(initialPosition.x + (i + 1) * 320, initialPosition.y + 90 + 60),
                thickness, true
            );

            createRotatingWall(
                Vec2(initialPosition.x + (i + 1) * 320, initialPosition.y - 90 - 60),
                Vec2(initialPosition.x + (i + 1) * 320, initialPosition.y - 90 + 60),
                thickness, false
            );
            break;

        case 5:
            createMovingWall(
                Vec2(initialPosition.x + (i + 1) * 320, initialPosition.y - 200),
                Vec2(initialPosition.x + (i + 1) * 320, initialPosition.y),
                thickness, 205
            );
            break;
        }
    }
}

// Walls

void GameScene::createVerticalStaticWall(cocos2d::Vec2 start, cocos2d::Vec2 end, float thickness, bool exit) {
    // Calculate the center position of the wall
    Vec2 center = (start + end) / 2;

    // Calculate the size of the wall
    float length = start.distance(end);  // Length is the distance between start and end
    Size size(length, thickness);

    // Create a node for the wall
    auto wall = Node::create();

    // Create a box physics body for the wall
    auto physicsBody = PhysicsBody::createBox(size);

    // Configure the physics body
    physicsBody->setDynamic(false);  // Make it static (doesn't move)
    // Wall physics body setup
    physicsBody->setCategoryBitmask(0x01);      // Walls are in category 1
    physicsBody->setContactTestBitmask(0x02);  // Walls trigger contact with particles (category 2)
    physicsBody->setCollisionBitmask(0x02);    // Walls collide with particles (category 2)

    // Make sure this collision mask is distinct from other objects (like walls)
    wall->setPhysicsBody(physicsBody);

    // Position the wall
    wall->setPosition(center);

    // Calculate the angle of rotation and set it
    float angle = atan2(end.y - start.y, end.x - start.x);
    wall->setRotation(-CC_RADIANS_TO_DEGREES(angle));

    // Visual representation using DrawNode
    auto drawNode = DrawNode::create();
    Vec2 vertices[4] = {
        Vec2(-length / 2, -thickness / 2),
        Vec2(length / 2, -thickness / 2),
        Vec2(length / 2, thickness / 2),
        Vec2(-length / 2, thickness / 2)
    };

    if (exit) {
        drawNode->drawSolidPoly(vertices, 4, Color4F::GREEN);
        wall->setTag(10);

        // Add collision detection for exit walls
        auto contactListener = EventListenerPhysicsContact::create();
        contactListener->onContactBegin = [this](PhysicsContact& contact) {
            auto shapeA = contact.getShapeA();
            auto shapeB = contact.getShapeB();
            auto nodeA = shapeA->getBody()->getNode();
            auto nodeB = shapeB->getBody()->getNode();

            // Check if a particle collided with the wall
            if (nodeA && nodeB) {
                Node* particleNode = nullptr;

                if (nodeB->getTag() == 100) std::swap(nodeB, nodeA);
                if (nodeA->getTag() == 100 && nodeB->getTag() == 10) particleNode = nodeA;

                if (particleNode) {
                    particleNode->setVisible(false);
                    particleNode->getPhysicsBody()->setEnabled(false);
                    solved++;
                    auto backgroundAudioId = AudioEngine::play2d("audio/Finish.mp3", false, 8.0f);
                    updateSolved();
                }
            }

            return true;
            };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, wall);
    }
    else {
        drawNode->drawSolidPoly(vertices, 4, Color4F::WHITE);
    }

    wall->addChild(drawNode);

    // Add the wall to the scene
    this->addChild(wall);
}

void GameScene::createHorizontalStaticWall(cocos2d::Vec2 start, cocos2d::Vec2 end, float thickness)
{
    // Calculate the center position of the wall
    Vec2 center = (start + end) / 2;

    // Calculate the size of the wall
    float length = start.distance(end);  // Length is the distance between start and end
    Size size(length, thickness);

    // Create a node for the wall
    auto wall = Node::create();

    // Create a box physics body for the wall
    auto physicsBody = PhysicsBody::createBox(size);

    // Configure the physics body
    physicsBody->setDynamic(false);  // Make it static (doesn't move)
    physicsBody->setContactTestBitmask(0xFFFFFFFF);  // Enable collision detection
    physicsBody->setCategoryBitmask(0x01);      // Walls are in category 1
    physicsBody->setContactTestBitmask(0x02);  // Walls trigger contact with particles (category 2)
    physicsBody->setCollisionBitmask(0x02);
    wall->setPhysicsBody(physicsBody);

    // Position the wall
    wall->setPosition(center);

    // Calculate the angle of rotation and set it
    float angle = atan2(end.y - start.y, end.x - start.x);
    wall->setRotation(-CC_RADIANS_TO_DEGREES(angle));

    // Visual representation using DrawNode
    auto drawNode = DrawNode::create();
    Vec2 vertices[4] = {
        Vec2(-length / 2, -thickness / 2),
        Vec2(length / 2, -thickness / 2),
        Vec2(length / 2, thickness / 2),
        Vec2(-length / 2, thickness / 2)
    };
    drawNode->drawSolidPoly(vertices, 4, Color4F::WHITE);
    wall->addChild(drawNode);

    // Add the wall to the scene
    this->addChild(wall);
}

void GameScene::createRotatingWall(cocos2d::Vec2 start, cocos2d::Vec2 end, float thickness, bool mov)
{
    // Calculate the center position of the wall
    Vec2 center = (start + end) / 2;

    // Calculate the size of the wall
    float length = start.distance(end);  // Length is the distance between start and end
    Size size(length, thickness);

    // Create a node for the wall
    auto wall = Node::create();

    // Create a box physics body for the wall
    auto physicsBody = PhysicsBody::createBox(size);

    // Configure the physics body
    physicsBody->setDynamic(false);  // Make it static so it doesn't react to forces
    physicsBody->setContactTestBitmask(0xFFFFFFFF);  // Enable collision detection
    physicsBody->setRotationEnable(true);  // Allow rotation for the physics body
    physicsBody->setCategoryBitmask(0x01);      // Walls are in category 1
    physicsBody->setContactTestBitmask(0x02);  // Walls trigger contact with particles (category 2)
    physicsBody->setCollisionBitmask(0x02);
    wall->setPhysicsBody(physicsBody);

    // Position the wall
    wall->setPosition(center);

    // Calculate the angle of rotation and set it initially
    float angle = atan2(end.y - start.y, end.x - start.x);
    wall->setRotation(-CC_RADIANS_TO_DEGREES(angle));

    // Visual representation using DrawNode
    auto drawNode = DrawNode::create();
    Vec2 vertices[4] = {
        Vec2(-length / 2, -thickness / 2),
        Vec2(length / 2, -thickness / 2),
        Vec2(length / 2, thickness / 2),
        Vec2(-length / 2, thickness / 2)
    };
    drawNode->drawSolidPoly(vertices, 4, Color4F::WHITE);
    wall->addChild(drawNode);

    // Add the wall to the scene
    this->addChild(wall);

    // Apply rotation action
    if (mov == true)
    {
        auto rotateAction = RotateBy::create(3.0f, 360);
        auto repeatAction = RepeatForever::create(rotateAction);
        wall->runAction(repeatAction);
    }
    else
    {
        auto rotateAction = RotateBy::create(3.0f, -360);
        auto repeatAction = RepeatForever::create(rotateAction);
        wall->runAction(repeatAction);
    }
}

void GameScene::createMovingWall(cocos2d::Vec2 start, cocos2d::Vec2 end, float thickness, float movementRange)
{
    // Calculate the center position of the wall
    Vec2 center = (start + end) / 2;

    // Calculate the size of the wall
    float length = start.distance(end);  // Length is the distance between start and end
    Size size(length, thickness);

    // Create a node for the wall
    auto wall = Node::create();

    // Create a box physics body for the wall
    auto physicsBody = PhysicsBody::createBox(size);

    // Configure the physics body
    physicsBody->setDynamic(false);  // Make it static so it doesn't react to forces
    physicsBody->setContactTestBitmask(0xFFFFFFFF);  // Enable collision detection
    physicsBody->setCategoryBitmask(0x01);      // Walls are in category 1
    physicsBody->setContactTestBitmask(0x02);  // Walls trigger contact with particles (category 2)
    physicsBody->setCollisionBitmask(0x02);
    wall->setPhysicsBody(physicsBody);

    // Position the wall
    wall->setPosition(center);

    // Calculate the angle of rotation and set it initially
    float angle = atan2(end.y - start.y, end.x - start.x);
    wall->setRotation(-CC_RADIANS_TO_DEGREES(angle));

    // Visual representation using DrawNode
    auto drawNode = DrawNode::create();
    Vec2 vertices[4] = {
        Vec2(-length / 2, -thickness / 2),
        Vec2(length / 2, -thickness / 2),
        Vec2(length / 2, thickness / 2),
        Vec2(-length / 2, thickness / 2)
    };
    drawNode->drawSolidPoly(vertices, 4, Color4F::WHITE);
    wall->addChild(drawNode);

    // Add the wall to the scene
    this->addChild(wall);

    // Apply movement action
    auto moveUp = MoveBy::create(2, Vec2(0, movementRange));
    auto moveDown = MoveBy::create(2, Vec2(0, -movementRange));
    auto movementSequence = Sequence::create(moveUp, moveDown, nullptr);
    auto repeatMovement = RepeatForever::create(movementSequence);
    wall->runAction(repeatMovement);
}


// Messages

void GameScene::generateMessages()
{
    // Get the visible size of the window and its origin
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Generation message
    generationLabel = Label::createWithTTF("Generation: 1", "fonts/regular.ttf", 140);
    generationLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 1 * visibleSize.height / 5));
    this->addChild(generationLabel, 1);

    // Quantity solved message
    solvedLabel = Label::createWithTTF("Solved by: 0", "fonts/regular.ttf", 140);
    solvedLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 4 * visibleSize.height / 5));
    this->addChild(solvedLabel, 1);

    // Return
    auto returnButton = Label::createWithTTF("Return", "fonts/Regular.ttf", 80);
    returnButton->setTextColor(stuffedLetters);
    auto returnItem = MenuItemLabel::create(returnButton, CC_CALLBACK_1(GameScene::returnCallback, this));
    returnItem->setAnchorPoint(Vec2(0, 1));
    returnItem->setPosition(Vec2(origin.x + 80, origin.y + visibleSize.height - 60));

    // Exit button
    auto exitButton = Label::createWithTTF("Exit", "fonts/Regular.ttf", 80);
    exitButton->setTextColor(stuffedLetters);
    auto exitItem = MenuItemLabel::create(exitButton, CC_CALLBACK_1(GameScene::exitCallback, this));
    exitItem->setAnchorPoint(Vec2(1, 1));
    exitItem->setPosition(Vec2(origin.x + visibleSize.width - 80, origin.y + visibleSize.height - 60));

    auto menu = Menu::create(returnItem, exitItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
}

void GameScene::updateGeneration()
{
    generationLabel->setString("Generation: " + std::to_string(iteration));
    return;
}

void GameScene::updateSolved()
{
    solvedLabel->setString("Solved by: " + std::to_string(solved));
    return;
}

// Callbacks

void GameScene::returnCallback(Ref* pSender)
{
    auto menuScene = DifficultyScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, menuScene));
}

void GameScene::exitCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

// Manage particles

void GameScene::handleUnscheduleParticle(Particle* particle)
{
    unscheduleCount++;
    if (unscheduleCount >= numParticles)
    {
        this->scheduleOnce([this](float)
            {
                restartParticles();
            }, 3.0f, "RestartParticlesDelay");
    }
}

void GameScene::restartParticles()
{
    if (iteration != 0) solutions.push_back(solved);
    if (iteration == 10)
    {
        showStatistics();
        return;
    }

    solved = 0;
    unscheduleCount = 0;
    iteration++;
    updateSolved();
    updateGeneration();

    // Get visible size and origin
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Just for the first iteration
    if (iteration == 1)
    {
        // Set the initial position
        initialPosition = Vec2(origin.x + 120 + 150, origin.y + visibleSize.height / 2);
        particles.resize(numParticles);
        for (auto& e : particles)
        {
            e = Particle::create(initialPosition);
            e->setTag(100);
            e->exit = origin.x + visibleSize.width - 120;
            e->start();
            e->onUnscheduleCallback = [this](Particle* p)
                {
                    handleUnscheduleParticle(p);
                };
            this->addChild(e);
        }

        return;
    }

    // Sort the solutions by the distance
    std::sort(particles.begin(), particles.end(),
        [](Particle* a, Particle* b) {
            return *a < *b;
        });

    // Create the new generation
    std::vector<Particle*> newParticles(numParticles);
    for (int i = 0; i < numParticles; ++i)
    {
        auto& e = newParticles[i];
        e = Particle::create(initialPosition);
        e->setTag(100);
        e->exit = origin.x + visibleSize.width - 120;
        e->deltaAngle = particles[i]->deltaAngle;
    }

    // Create the next generation in newParticles
    newGeneration(newParticles);

    // Clean up old particles
    for (auto& particle : particles)
    {
        if (particle)
        {
            particle->removeFromParent(); // Removes from scene and decrements reference count
        }
    }
    particles.clear();

    // Recreate the particles vector
    particles.resize(numParticles);
    for (int i = 0; i < numParticles; ++i)
    {
        particles[i] = newParticles[i];
        particles[i]->start();
        particles[i]->onUnscheduleCallback = [this](Particle* p)
            {
                handleUnscheduleParticle(p);
            };
        this->addChild(particles[i]);
    }

    return;
}

void GameScene::newGeneration(std::vector<Particle*>& newParticles)
{
    // The best 20% remains the same
    for (int i = 0; i < 20; ++i) newParticles[i]->deltaAngle = particles[i]->deltaAngle;

    // The following 50% 
    for (int i = 20; i < 70; i+=2)
    {
        mergeSolutions(newParticles[i]->deltaAngle,
            newParticles[i + 1]->deltaAngle,
            particles[i - 20]->deltaAngle,
            particles[i - 20 + 1]->deltaAngle);

        // Since they are the combination of several solutions, mutate them
        newParticles[i]->mutation();
        newParticles[i+1]->mutation();
    }

    // The rest 30% are new particles
    for (int i = 70; i < numParticles; ++i)
    {
        // Basically do nothing
        newParticles[i] = newParticles[i];
    }

    return;
}

void GameScene::mergeSolutions(std::vector<float>& a, std::vector<float>& b, const std::vector<float>& x, const std::vector<float>& y)
{
    // Point where to separate solutions
    int p = std::rand() % a.size();

    for (int i = 0; i < (int)a.size(); ++i)
    {
        if (i >= p)
        {
            b[i] = x[i];
            a[i] = y[i];
        }
        else
        {
            a[i] = x[i];
            b[i] = y[i];
        }
    }
}

// Show results

void GameScene::showStatistics()
{
    auto statisticScene = new StatisticScene(solutions);
    statisticScene->init();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, statisticScene));

    return;
}