#pragma once

#include <iostream>

#include "utils/directions.hpp"
#include "utils/behavior.hpp"
#include "animatedSprite.hpp"

#include <map>
#include <vector>

class Character {
    public:
        Character(float sizeX, float sizeY);

        void addAnimation(BehaviorType animation, Direction direction, const std::string& filename, int columns, int rows, int framerate);
        void draw(double deltaTime, BehaviorType animation, Direction direction);

    private:
        std::map<BehaviorType, std::map<Direction, AnimatedSprite*>> sprites;
        Direction direction {Direction::NORTH};
        BehaviorType animation {BehaviorType::IDLE};
        float sizeX;
        float sizeY;
};