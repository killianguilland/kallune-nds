#include "player.hpp"
#include <cmath>
#include <logic/Map/map.hpp>
#include "utils/directions.hpp"
#include "utils/behavior.hpp"

Player::Player(float startX, float startY, Map& map)
    : map(map), x(startX), y(startY), speed(5.f), tileSize(1.0f), alive(true), score(0)
{
}

void Player::move(float dirX, float dirY, float deltaTime)
{
    float length = std::sqrt(dirX * dirX + dirY * dirY);
    if (length > 0.0f) {
        dirX /= length;
        dirY /= length;

        const float tileSpeed {map.getSpeed(map.getMap()[getTileX()][getTileY()])};
        
        x += dirX * speed * tileSpeed * deltaTime;
        y += dirY * speed * tileSpeed * deltaTime;
    }

    int tileX = getTileX();
    int tileY = getTileY();
    if (tileX >= 0 && tileY >= 0 && tileX < map.getWidth() && tileY < map.getHeight()) {
        if (map.getMap()[tileX][tileY] == MapType::FLOWER) { 
            addScore(1);
            map.removeFlower(tileX, tileY);
        }
    }
}

void Player::kill()
{
    alive = false;
}

void Player::setPosition(float newX, float newY)
{
    x = newX;
    y = newY;
}

float Player::getX() const
{
    return x;
}

float Player::getY() const
{
    return y;
}

int Player::getTileX() const
{
    return static_cast<int>(x / tileSize);
}

int Player::getTileY() const
{
    return static_cast<int>(y / tileSize);
}

bool Player::isAlive() const
{
    return alive;
}

float Player::getSpeed() const
{
    return speed;
}

int Player::getScore() const
{
    return score;
}

void Player::addScore(int points)
{
    score += points;
}

void Player::setDirection(Direction newDirection)
{
    direction = newDirection;
}

void Player::calculateBehavior(float dirX, float dirY)
{
        if (dirX == 0.0f && dirY == 0.0f) {
            behavior = BehaviorType::IDLE;
        } else {
            behavior = BehaviorType::MOVE;
        }
}

bool Player::canMine() const {
    return miningCooldown <= 0.0f && alive;
}

bool Player::mine() {
    if (!canMine()) {
        return false;
    }
    std::cout << "Player is mining at: (" << getTileX() << ", " << getTileY() << ")" << std::endl;
    int targetX = getTileX();
    int targetY = getTileY();
    
    switch (direction) {
        case Direction::NORTH: targetY -= 1; break;
        case Direction::SOUTH: targetY += 1; break;
        case Direction::EAST:  targetX += 1; break;
        case Direction::WEST:  targetX -= 1; break;
    }
    
    std::cout << "Target tile for mining: (" << targetX << ", " << targetY << ")" << std::endl;

    if (targetX >= 0 && targetY >= 0 && targetX < map.getWidth() && targetY < map.getHeight()) {
        if (map.getMap()[targetX][targetY] == MapType::WALL) {
            map.changeTile(targetX, targetY, MapType::GRASS);
            miningCooldown = maxMiningCooldown;
            
            return true;
        }
    }
    
    return false;
}

void Player::update(float deltaTime) {
    if (miningCooldown > 0.0f) {
        miningCooldown -= deltaTime;
        if (miningCooldown < 0.0f) {
            miningCooldown = 0.0f;
        }
    }
}