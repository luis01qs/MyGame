#include "Particle.h"
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>

USING_NS_CC;

// Constructor
Particle::Particle(Vec2 position)
{
    // Initialize variables
    speed = 80;
    angle = 0;
    distance = 100000;
    this->position = position;
    this->currentIndex = 0;
    this->currentIteration = 0;

    // Create Sprite
    sprite = Sprite::create("sprites/particle.png");
    sprite->setOpacity(100);
    sprite->setScaleX(0.6f);
    sprite->setScaleY(0.8f);
    sprite->setTag(100);
    if (sprite) this->addChild(sprite);

    // Set the position and orientation of the Sprite
    this->setPosition(position);

    // Define the isosceles triangle vertices
    float base = sprite->getContentSize().width;  // Width of the triangle base
    float height = sprite->getContentSize().height; // Height of the triangle
    Vec2 vertices[] = {
        Vec2(-base / 2, -height / 2), // Bottom-left corner
        Vec2(base / 2, -height / 2),  // Bottom-right corner
        Vec2(0, height / 2)           // Top vertex
    };

    // Add physics body as a polygon
    physicsBody = PhysicsBody::createPolygon(vertices, 3);
    physicsBody->setDynamic(true);           // Make the particle dynamic
    physicsBody->setRotationEnable(false);   // Disable automatic rotation
    physicsBody->setGravityEnable(true);     // Optionally enable gravity
    physicsBody->setLinearDamping(0.5f);     // Apply damping to the particle's motion
    // Particle physics body setup
    physicsBody->setCategoryBitmask(0x02);      // Particles are in category 2
    physicsBody->setContactTestBitmask(0x01);  // Particles should only trigger contact with walls (category 1)
    physicsBody->setCollisionBitmask(0x01);    // Particles should only collide with walls (category 1)

    physicsBody->setTag(100);
    // Make sure this collision mask is distinct from other objects (like walls)

    sprite->setPhysicsBody(physicsBody);

    // Random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dist(0.0f, 6.0f);

    // Generate randomly the solution
    this->vectorSize = 10;
    this->deltaAngle.resize(vectorSize);
    for (int i = 0; i < vectorSize; ++i)
    {
        deltaAngle[i] = dist(gen) * 3;
    }
}

// Initialization
bool Particle::init() 
{
    if (!Node::init()) return false;
    //scheduleUpdate();
    return true;
}

// Update method
void Particle::update(float deltaTime) 
{
    //CCLOG("%d", currentIndex);
    if (this->physicsBody == nullptr || currentIndex >= vectorSize)
    {
        if (sprite->getOpacity() > 0)
        {
            sprite->setOpacity(sprite->getOpacity() - 1);
        }
        else
        {
            unscheduleUpdate();
            if (onUnscheduleCallback) {
                onUnscheduleCallback(this);
            }
        }
        return;
    }

    if (currentIteration % 10 == 0)
    {
        angle += deltaAngle[currentIndex];
        currentIndex++;
    }

    if (angle >= 360.0f) angle -= 360;
    if (angle < 0.0f) angle += 360;
    
    float radians = CC_DEGREES_TO_RADIANS(angle);
    Vec2 direction(std::cos(radians), std::sin(radians));
    velocity = direction * speed;

    // Update position
    Vec2 newPosition = getPosition() + velocity * deltaTime;
    setPosition(newPosition);

    // Rotate the sprite to match the movement direction
    if(sprite) sprite->setRotation(-angle + 90);

    // Next iteration
    currentIteration++;

    // Update the distance to the exit
    if (distance > exit - getPosition().x) distance = exit - getPosition().x;
    if (distance == 0) distance = std::min(distance, distance + deltaAngle.size() * 10 - currentIteration);
}

// Start the movement
void Particle::start()
{
    scheduleUpdate();
    return;
}

// Factory method
Particle* Particle::create(Vec2 position) 
{
    Particle* particle = new (std::nothrow) Particle(position);
    if (particle && particle->init()) {
        particle->autorelease();
        return particle;
    }
    CC_SAFE_DELETE(particle);
    return nullptr;
}

// Mutate method
void Particle::mutation()
{
    // How many mutations are going to be
    int num = rand() % (int)deltaAngle.size() / 5;

    // Random stuff
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dist(0.0f, 2.0f);

    // Do them
    while (num--)
    {
        int x = rand() % (int)deltaAngle.size();
        deltaAngle[x] += dist(gen);
    }
}