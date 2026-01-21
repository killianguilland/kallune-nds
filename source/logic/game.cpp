#include "game.hpp"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <optional>
#include <random>
#include <set>

Game::Game(int waterValue, int sizeValue, int typeValue)
    : map(waterValue, sizeValue, typeValue), flowField(map.getWidth(), map.getHeight()), player(50.0f, 50.0f, map)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    auto positionOpt = getRandomPlacablePosition();
    if (positionOpt.has_value())
    {
        auto [x, y] = positionOpt.value();
        occupiedTiles.insert({x, y});
        player.setPosition(static_cast<float>(x), static_cast<float>(y));
        fprintf(stderr, "Player placed at: (%d, %d)\n", x, y);
    }
    else
    {
        fprintf(stderr, "Erreur : impossible de placer le joueur (aucune case disponible).\n");
        player.setPosition(50.0f, 50.0f);
    }

    updateFlowField();
    generateEntities(7, 3, 4);
}

void Game::handlePlayerMovement(const InputState& inputState)
{
    // if (inputState.moveX == 0.0f && inputState.moveY == 0.0f)
    // {
    //     player.calculateBehavior(inputState.moveX, inputState.moveY);
    //     return;
    // }

    // if (inputState.moveX > 0.0f)
    //     player.setDirection(Direction::EAST);
    // else if (inputState.moveX < 0.0f)
    //     player.setDirection(Direction::WEST);
    // else if (inputState.moveY > 0.0f)
    //     player.setDirection(Direction::SOUTH);
    // else if (inputState.moveY < 0.0f)
    //     player.setDirection(Direction::NORTH);

    // float nextX = player.getX() + inputState.moveX * player.getSpeed();
    // float nextY = player.getY() + inputState.moveY * player.getSpeed();

    // int tileX = static_cast<int>(nextX);
    // int tileY = static_cast<int>(nextY);

    // int tileXOffset = static_cast<int>(nextX + (inputState.moveX < 0 ? -0.2f : 0.2f));
    // int tileYOffset = static_cast<int>(nextY + (inputState.moveY < 0 ? -0.2f : 0.2f));

    // if (map.getMap()[tileX][tileY] == MapType::WATER)
    // {
    //     // player.kill();
    // }

    // if (!map.isWalkable(tileXOffset, tileY))
    // {
    //     // inputState.moveX = 0.0f;
    // }
    // if (!map.isWalkable(tileX, tileYOffset))
    // {
    //     // inputState.moveY = 0.0f;
    // }

    player.move(inputState.moveX, inputState.moveY);
    // player.calculateBehavior(inputState.moveX, inputState.moveY);

    // if (inputState.keyStates[GLFW_KEY_E] == GLFW_PRESS || inputState.keyStates[GLFW_KEY_E] == GLFW_REPEAT)
    // {
    //     player.mine();
    // }
}

void Game::update(InputState inputState)
{
    // cpuStartTiming(0);
    if (player.isAlive())
    {
        handlePlayerMovement(inputState);

        // player.update();
        // updateFlowField();

        // updateEntities();
    }

    // u32   cycles = cpuEndTiming();
    // float usage  = (cycles / 1120380.0f) * 100.0f;
    // fprintf(stderr, "LOGIC BUDGET: %f%%   \n", usage);
}

std::vector<EntityInfo> Game::getEntitiesInfo() const
{
    std::vector<EntityInfo> infos;
    infos.reserve(entities.size());
    for (const auto& e : entities)
    {
        infos.emplace_back(EntityInfo{e->getX(), e->getY(), e->isAlive(), e->isAggressive(), e->getType(), e->getDirection(), e->getBehavior()});
    }

    return infos;
}

bool Game::isKeyPressed(int keyCode) const
{
    auto it = keyStates.find(keyCode);
    return it != keyStates.end() && it->second;
}

void Game::generateEntities(int countWolf, int countBoar, int countDeer)
{
    // for (int i = 0; i < countWolf; ++i)
    // {
    //     Wolf *wolf = new Wolf(0, 0, &player, &flowField);
    //     placeEntityRandomly(wolf);
    //     entities.push_back(wolf);
    // }
    // for (int i = 0; i < countBoar; ++i)
    // {
    //     Boar *boar = new Boar(0, 0, &player, &flowField);
    //     placeEntityRandomly(boar);
    //     entities.push_back(boar);
    // }
    // for (int i = 0; i < countDeer; ++i)
    // {
    //     Deer *deer = new Deer(0, 0, &player, &flowField);
    //     placeEntityRandomly(deer);
    //     entities.push_back(deer);
    // }
}

Game::~Game()
{
    // for (auto entity : entities)
    // {
    // delete entity;
    // }
}

std::optional<std::pair<int, int>> Game::getRandomPlacablePosition()
{
    const auto& grid   = map.getMap();
    int         width  = map.getWidth();
    int         height = map.getHeight();

    std::vector<std::pair<int, int>> validPositions;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            MapType type = grid[y * width + x];
            if ((type == MapType::GRASS || type == MapType::SAND || type == MapType::FLOWER) && occupiedTiles.find({x, y}) == occupiedTiles.end())
            {
                validPositions.emplace_back(x, y);
            }
        }
    }

    if (validPositions.empty())
    {
        return std::nullopt;
    }

    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, validPositions.size() - 1);

    return validPositions[dis(gen)];
}

void Game::placeEntityRandomly(Entity* entity)
{
    auto positionOpt = getRandomPlacablePosition();
    if (!positionOpt.has_value())
    {
        std::cerr << "Erreur : aucune case valide disponible pour placer l'entité." << std::endl;
        return;
    }

    auto [tileX, tileY] = positionOpt.value();
    occupiedTiles.insert({tileX, tileY});

    float posX = static_cast<float>(tileX);
    float posY = static_cast<float>(tileY);

    fprintf(stderr, "Placing entity at: (%.2f, %.2f)\n", posX, posY);

    entity->setPosition(posX, posY);
}

bool Game::isWalkableTile(int x, int y) const
{
    if (x < 0 || y < 0 || x >= map.getWidth() || y >= map.getHeight())
        return false;
    float speed = map.getSpeedMap()[y * map.getWidth() + x];
    return speed > 0.0f;
}

void Game::updateFlowField()
{
    int px = static_cast<int>(player.getX());
    int py = static_cast<int>(player.getY());
    flowField.computeFlowField(px, py, map);
}

void Game::updateEntities()
{
    for (auto& entity : entities)
    {
        entity->decideBehavior(player);
        entity->update();
    }
}

bool Game::isPlayerAlive()
{
    return player.isAlive();
}