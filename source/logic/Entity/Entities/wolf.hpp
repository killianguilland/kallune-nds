#pragma once
#include "../../Flowfield/flowfield.hpp"
#include "../../Player/player.hpp"
#include "../entity.hpp"

class Wolf : public Entity {
public:
    Wolf(float startX, float startY);
    Wolf(float startX, float startY, const Player* player, const FlowField* flowField);

    void update() override;
    void decideBehavior(Player& player) override;

private:
    bool aggressive() const override { return true; }
    bool coward() const override { return false; }

    float pursuitRange = 30.f;
};