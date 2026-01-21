#pragma once

#include <logic/Map/map.hpp>
#include "utils/behavior.hpp"
#include "utils/directions.hpp"

class Player {
public:
    Player(float startX, float startY, Map& map);

    void move(int32_t dirX, int32_t dirY);
    void takeDamage(float amount);

    void setPosition(float newX, float newY);
    // float getX() const;
    // float getY() const;
    // int   getTileX() const;
    // int   getTileY() const;

    // Accesseurs
    int32_t getX() const { return x; }
    int32_t getY() const { return y; }

    // Coordonnées entières pour la logique de grille
    int getTileX() const { return x >> FP_SHIFT; }
    int getTileY() const { return y >> FP_SHIFT; }

    bool isAlive() const;
    void kill();

    float getSpeed() const;

    int  getScore() const;
    void addScore(int points);

    void resetScore() { score = 0; }

    void calculateBehavior(float dirX, float dirY);

    Direction getDirection() const { return direction; }

    BehaviorType getBehavior() const { return behavior; }

    bool mine();
    bool canMine() const;

    void update();

    void setDirection(Direction newDirection);

private:
    int32_t   x, y; // Stockés en Fixed Point (format 24.8)
    const int FP_SHIFT = 8;
    const int FP_ONE   = (1 << FP_SHIFT);
    // float x, y;
    float speed;
    bool  alive    = true;
    float tileSize = 1.0f;

    int  score = 0;
    Map& map;

    Direction    direction = Direction::SOUTH;
    BehaviorType behavior  = BehaviorType::IDLE;

    float       miningCooldown    = 0.0f;
    const float maxMiningCooldown = 0.8f;
};
