#include "player.hpp"
#include <nds.h>
#include <stdio.h>
#include <cmath>
#include <logic/Map/map.hpp>
#include "utils/behavior.hpp"
#include "utils/directions.hpp"

#define FP_SHIFT 8
#define FP_ONE   (1 << FP_SHIFT)

Player::Player(float startX, float startY, Map& map)
    : map(map), x(startX), y(startY), speed(5.f), tileSize(1.0f), alive(true), score(0)
{
}

void Player::move(int32_t dirX, int32_t dirY)
{
    // dirX et dirY arrivent en tant qu'entiers (-1, 0, 1) ou en Fixed Point
    if (dirX == 0 && dirY == 0)
        return;

    // 1. Calcul de la longueur au carré (dx*dx + dy*dy)
    // On travaille en entiers pour éviter le float
    int32_t l2     = dirX * dirX + dirY * dirY;
    int32_t length = 0;

    if (l2 > 0)
    {
        // 2. Utilisation de la fonction BIOS ultra-rapide pour la racine carrée
        length = swiSqrt(l2 << FP_SHIFT); // On décale pour garder de la précision

        // 3. Normalisation et application de la vitesse
        // On récupère la vitesse du sol (en Fixed Point également)
        int32_t tileX = getTileX();
        int32_t tileY = getTileY();

        // Optimisation : On ne fait l'accès à la map qu'une seule fois
        // TODO : Fix seg fault
        // int32_t tileSpeed = map.getSpeed(map.getMap()[tileX][tileY]);

        // Calcul final : (Direction * Vitesse * VitesseSol) / Longueur
        // On utilise des décalages de bits pour maintenir la précision du Fixed Point
        x += (dirX * speed) / length;
        y += (dirY * speed) / length;
    }

    // Gestion des collisions/fleurs (décommenté et optimisé)
    // Vérification des bords de map simplifiée
    if ((uint32_t)getTileX() < (uint32_t)map.getWidth() && (uint32_t)getTileY() < (uint32_t)map.getHeight())
    {
        if (map.getMap()[getTileX()][getTileY()] == MapType::FLOWER)
        {
            addScore(1);
            map.removeFlower(getTileX(), getTileY());
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
    if (dirX == 0.0f && dirY == 0.0f)
    {
        behavior = BehaviorType::IDLE;
    }
    else
    {
        behavior = BehaviorType::MOVE;
    }
}

bool Player::canMine() const
{
    return miningCooldown <= 0.0f && alive;
}

bool Player::mine()
{
    if (!canMine())
    {
        return false;
    }
    fprintf(stderr, "Player is mining at: (%d, %d)\n", getTileX(), getTileY());
    int targetX = getTileX();
    int targetY = getTileY();

    switch (direction)
    {
    case Direction::NORTH: targetY -= 1; break;
    case Direction::SOUTH: targetY += 1; break;
    case Direction::EAST: targetX += 1; break;
    case Direction::WEST: targetX -= 1; break;
    }

    fprintf(stderr, "Target tile for mining: (%d, %d)\n", targetX, targetY);

    if (targetX >= 0 && targetY >= 0 && targetX < map.getWidth() && targetY < map.getHeight())
    {
        if (map.getMap()[targetX][targetY] == MapType::WALL)
        {
            map.changeTile(targetX, targetY, MapType::GRASS);
            miningCooldown = maxMiningCooldown;

            return true;
        }
    }

    return false;
}

void Player::update()
{
    if (miningCooldown > 0.0f)
    {
        miningCooldown -= 1 / 60.0f;
        if (miningCooldown < 0.0f)
        {
            miningCooldown = 0.0f;
        }
    }
}