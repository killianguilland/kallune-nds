#pragma once
#include "../../Player/player.hpp"
#include "../../Flowfield/flowfield.hpp"
#include "../entity.hpp"
#include <cmath>

class Deer : public Entity
{
public:
    Deer(float startX, float startY);
    Deer(float startX, float startY, const Player* player, const FlowField* flowField);

    void update(float deltaTime) override;
    void decideBehavior(Player& player) override;

private:
    bool aggressive() const override { return false; }
    bool coward() const override { return true; }

    float fleeDistance = 3.0f;
};