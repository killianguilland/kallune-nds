#include "character.hpp"

Character::Character(float sizeX, float sizeY)
    : sizeX(sizeX), sizeY(sizeY) {}

void Character::addAnimation(BehaviorType animation, Direction direction, const std::string& filename, int columns, int rows, int framerate) {
    sprites[animation][direction] = new AnimatedSprite(filename, sizeX, sizeY, columns, rows, framerate);
}

void Character::draw(double deltaTime, BehaviorType animation, Direction direction) {
    sprites[animation][direction]->update(deltaTime);
    sprites[animation][direction]->draw();
}