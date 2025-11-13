#pragma once

#include "utils/entityType.hpp"
#include "utils/behavior.hpp"
#include "logic/Player/player.hpp"
#include "logic/Flowfield/flowfield.hpp"

class Entity {
public:
    Entity(float startX, float startY);
    virtual ~Entity() = default;

    virtual void update(float deltaTime) = 0;
    virtual void decideBehavior( Player& player) = 0;

    void setFlowField(const FlowField* field);

    float getX() const;
    float getY() const;
    int getTileX() const;
    int getTileY() const;

    bool isAlive() const;
    bool isAggressive() const;
    bool isCoward() const;

    void takeDamage(float dmg); 
    void setPlayer(const Player* p);
    void setPosition(float newX, float newY);

    BehaviorType getBehavior() const { return behavior; }
    Direction getDirection() const;
    EntityType getType() const;
protected:
    float x, y;
    float speed = 1.5f;
    float health = 100.0f;
    float tileSize = 1.0f;
    EntityType type;
    const FlowField* flowField = nullptr;
    BehaviorType behavior = BehaviorType::IDLE;

    virtual bool aggressive() const = 0;
    virtual bool coward() const = 0;

    const Player* player = nullptr;
    Direction direction = Direction::NORTH;

    void calculateDirection(float dirX, float dirY);
};
