#include "game.hpp"
#include <cstdlib>
#include <ctime>
#include <random>
#include <set>
#include <optional>

Game::Game()
    : map(), flowField(map.getWidth(), map.getHeight()), player(50.0f, 50.0f, map)
{

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    auto positionOpt = getRandomPlacablePosition();
    if (positionOpt.has_value())
    {
        auto [x, y] = positionOpt.value();
        occupiedTiles.insert({x, y});
        player.setPosition(static_cast<float>(x), static_cast<float>(y));
        std::cout << "Player placed at: (" << x << ", " << y << ")" << std::endl;
    }
    else
    {
        std::cerr << "Erreur : impossible de placer le joueur (aucune case disponible)." << std::endl;
        player.setPosition(50.0f, 50.0f);
    }

    updateFlowField();
    generateEntities(7, 3, 4);
}

void Game::handlePlayerMovement(const InputState &inputState, float deltaTime)
{
    float dirX = 0.0f;
    float dirY = 0.0f;

    if (inputState.keyStates[GLFW_KEY_W] == GLFW_PRESS || inputState.keyStates[GLFW_KEY_W] == GLFW_REPEAT)
    {
        dirY += 1.0f;
    }
    if (inputState.keyStates[GLFW_KEY_S] == GLFW_PRESS || inputState.keyStates[GLFW_KEY_S] == GLFW_REPEAT)
    {
        dirY -= 1.0f;
    }
    if (inputState.keyStates[GLFW_KEY_A] == GLFW_PRESS || inputState.keyStates[GLFW_KEY_A] == GLFW_REPEAT)
    {
        dirX -= 1.0f;
    }
    if (inputState.keyStates[GLFW_KEY_D] == GLFW_PRESS || inputState.keyStates[GLFW_KEY_D] == GLFW_REPEAT)
    {
        dirX += 1.0f;
    }

    if (dirX == 0.0f && dirY == 0.0f)
    {
        player.calculateBehavior(dirX, dirY);
        return;
    }

    if (dirX > 0.0f)
        player.setDirection(Direction::EAST);
    else if (dirX < 0.0f)
        player.setDirection(Direction::WEST);
    else if (dirY > 0.0f)
        player.setDirection(Direction::SOUTH);
    else if (dirY < 0.0f)
        player.setDirection(Direction::NORTH);

    float nextX = player.getX() + dirX * player.getSpeed() * deltaTime;
    float nextY = player.getY() + dirY * player.getSpeed() * deltaTime;

    int tileX = static_cast<int>(nextX);
    int tileY = static_cast<int>(nextY);

    int tileXOffset = static_cast<int>(nextX + (dirX < 0 ? -0.2f : 0.2f));
    int tileYOffset = static_cast<int>(nextY + (dirY < 0 ? -0.2f : 0.2f));

    if (map.getMap()[tileX][tileY] == MapType::WATER)
    {
        player.kill();
    }

    if (!map.isWalkable(tileXOffset, tileY))
    {
        dirX = 0.0f;
    }
    if (!map.isWalkable(tileX, tileYOffset))
    {
        dirY = 0.0f;
    }

    player.move(dirX, dirY, deltaTime);
    player.calculateBehavior(dirX, dirY);

    if (inputState.keyStates[GLFW_KEY_E] == GLFW_PRESS || inputState.keyStates[GLFW_KEY_E] == GLFW_REPEAT)
    {
        player.mine();
    }
}

void Game::update(float deltaTime, InputState inputState)
{
    if (player.isAlive())
    {
    handlePlayerMovement(inputState, deltaTime);

    player.update(deltaTime);
    updateFlowField();

    updateEntities(deltaTime);
    }
}

float Game::getPlayerX() const
{
    return player.getX();
}

float Game::getPlayerY() const
{
    return player.getY();
}

std::vector<EntityInfo> Game::getEntitiesInfo() const
{
    std::vector<EntityInfo> infos;
    infos.reserve(entities.size());
    for (const auto &e : entities)
    {
        infos.emplace_back(EntityInfo{
            e->getX(),
            e->getY(),
            e->isAlive(),
            e->isAggressive(),
            e->getType(),
            e->getDirection(),
            e->getBehavior()});
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
    for (int i = 0; i < countWolf; ++i)
    {
        Wolf *wolf = new Wolf(0, 0, &player, &flowField);
        placeEntityRandomly(wolf);
        entities.push_back(wolf);
    }
    for (int i = 0; i < countBoar; ++i)
    {
        Boar *boar = new Boar(0, 0, &player, &flowField);
        placeEntityRandomly(boar);
        entities.push_back(boar);
    }
    for (int i = 0; i < countDeer; ++i)
    {
        Deer *deer = new Deer(0, 0, &player, &flowField);
        placeEntityRandomly(deer);
        entities.push_back(deer);
    }
}

Game::~Game()
{
    for (auto entity : entities)
    {
        // delete entity;
    }
}

std::optional<std::pair<int, int>> Game::getRandomPlacablePosition()
{
    const auto &grid = map.getMap();
    int width = map.getWidth();
    int height = map.getHeight();

    std::vector<std::pair<int, int>> validPositions;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {

            MapType type = grid[x][y];
            if ((type == MapType::GRASS || type == MapType::SAND || type == MapType::FLOWER) &&
                occupiedTiles.find({x, y}) == occupiedTiles.end())
            {
                validPositions.emplace_back(x, y);
            }
        }
    }

    if (validPositions.empty())
    {
        return std::nullopt;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, validPositions.size() - 1);

    return validPositions[dis(gen)];
}

void Game::placeEntityRandomly(Entity *entity)
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

    std::cout << "Placing entity at: (" << posX << ", " << posY << ")" << std::endl;

    entity->setPosition(posX, posY);
}

bool Game::isWalkableTile(int x, int y) const
{
    if (x < 0 || y < 0 || x >= map.getWidth() || y >= map.getHeight())
        return false;
    float speed = map.getSpeedMap()[x][y];
    return speed > 0.0f;
}

void Game::updateFlowField()
{
    int px = static_cast<int>(player.getX());
    int py = static_cast<int>(player.getY());
    flowField.computeFlowField(px, py, map);
}

void Game::updateEntities(float deltaTime)
{
    for (auto &entity : entities)
    {
        entity->decideBehavior(player);
        entity->update(deltaTime);
    }
}

bool Game::isPlayerAlive()
{
    return player.isAlive();
}