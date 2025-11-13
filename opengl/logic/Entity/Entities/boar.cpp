#include "boar.hpp"
#include "utils/entityType.hpp"

Boar::Boar(float startX, float startY) : Entity(startX, startY)
{
    speed = 1.2f;
    type = EntityType::BOAR;
}

Boar::Boar(float startX, float startY, const Player *player, const FlowField *flowField)
    : Entity(startX, startY)
{
    this->player = player;
    this->flowField = flowField;
    speed = 1.2f;
        type = EntityType::BOAR;

}

void Boar::decideBehavior(Player &player)
{
    float dx = player.getX() - x;
    float dy = player.getY() - y;
    float distanceSq = std::sqrt(dx * dx + dy * dy);

    if (distanceSq < 0.5f)
    {
        player.kill();
    }
    else if (distanceSq < pursuitRange)
    {
        behavior = BehaviorType::ATTACK;
    }
    else
    {
        behavior = BehaviorType::IDLE;
    }
}

void Boar::update(float deltaTime)
{
    if (!flowField || !isAlive())
    {
        return;
    }

    switch (behavior)
    {
    case BehaviorType::ATTACK:
    {
        int tileX = getTileX();
        int tileY = getTileY();
        float dirX = 0.0f, dirY = 0.0f;

        flowField->getDirectionAt(tileX, tileY, dirX, dirY);

        float length = std::sqrt(dirX * dirX + dirY * dirY);
        if (length > 0.0f)
        {
            dirX /= length;
            dirY /= length;

            x += dirX * speed * deltaTime;
            y += dirY * speed * deltaTime;

            calculateDirection(dirX, dirY);
        }
        break;
    }
    case BehaviorType::IDLE:
    {
        calculateDirection(0, 0);

        break;
    }
    case BehaviorType::FLEE:
    {
        break;
    }
    }
}
