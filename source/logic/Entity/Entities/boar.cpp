#include "boar.hpp"
#include "utils/entityType.hpp"
// #include <cmath>

Boar::Boar(float startX, float startY) : Entity(startX, startY)
{
    speed = 1.2f;
    type  = EntityType::BOAR;
}

Boar::Boar(float startX, float startY, const Player* player, const FlowField* flowField)
    : Entity(startX, startY)
{
    this->player    = player;
    this->flowField = flowField;
    speed           = 1.2f;
    type            = EntityType::BOAR;
}

void Boar::decideBehavior(Player& player)
{
    float dx = player.getX() - x;
    float dy = player.getY() - y;

    // On reste au carré (Square Distance)
    float distSq = (dx * dx) + (dy * dy);

    // 0.5f au carré = 0.25f
    if (distSq < 0.25f)
    {
        player.kill();
    }
    // pursuitRange au carré (à calculer une fois idéalement)
    else if (distSq < (pursuitRange * pursuitRange))
    {
        behavior = BehaviorType::ATTACK;
    }
    else
    {
        behavior = BehaviorType::IDLE;
    }
}

void Boar::update()
{
    // if (!flowField || !isAlive())
    // {
    //     return;
    // }

    // switch (behavior)
    // {
    // case BehaviorType::ATTACK:
    // {
    //     int   tileX = getTileX();
    //     int   tileY = getTileY();
    //     float dirX = 0.0f, dirY = 0.0f;

    //     flowField->getDirectionAt(tileX, tileY, dirX, dirY);

    //     float length = std::sqrt(dirX * dirX + dirY * dirY);
    //     if (length > 0.0f)
    //     {
    //         dirX /= length;
    //         dirY /= length;

    //         x += dirX * speed;
    //         y += dirY * speed;

    //         calculateDirection(dirX, dirY);
    //     }
    //     break;
    // }
    // case BehaviorType::IDLE:
    // {
    //     calculateDirection(0, 0);

    //     break;
    // }
    // case BehaviorType::FLEE:
    // {
    //     break;
    // }
    // case BehaviorType::MOVE:
    // {
    //     break;
    // }
    // }
}
