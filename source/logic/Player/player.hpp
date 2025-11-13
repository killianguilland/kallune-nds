#pragma once

#include <logic/Map/map.hpp>
#include "utils/directions.hpp"
#include "utils/behavior.hpp"

class Player
{
public:
    Player(float startX, float startY, Map& map);

    void move(float dirX, float dirY, float deltaTime);
    void takeDamage(float amount);

    void setPosition(float newX, float newY);
    float getX() const;
    float getY() const;
    int getTileX() const;
    int getTileY() const;

    bool isAlive() const;
    void kill();

    float getSpeed() const;

    int getScore() const;
    void addScore(int points);

    void resetScore() { score = 0; }

    void calculateBehavior(float dirX, float dirY);

    Direction getDirection() const { return direction; }

    BehaviorType getBehavior() const { return behavior; }

    bool mine();  
    bool canMine() const;

    void update(float deltaTime);

    void setDirection(Direction newDirection);
private:
    float x, y;
    float speed;
    bool alive = true;
    float tileSize = 1.0f;

    int score = 0;
    Map& map;

    Direction direction = Direction::SOUTH;
    BehaviorType behavior = BehaviorType::IDLE;

    float miningCooldown = 0.0f;        
    const float maxMiningCooldown = 0.8f; 
};
