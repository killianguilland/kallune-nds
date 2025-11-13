#pragma once
#include "../../Player/player.hpp"
#include "../../Flowfield/flowfield.hpp"
#include "../entity.hpp"
#include <cmath>

class Boar : public Entity
{
public:
    Boar(float startX, float startY);
    Boar(float startX, float startY, const Player* player, const FlowField* flowField);

    void update(float deltaTime) override;
    void decideBehavior(Player& player) override;

private:
    bool aggressive() const override { return true; }
    bool coward() const override { return false; }

    float pursuitRange = 5.f;
};