#pragma once

#include <memory>
#include <optional>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>
#include "Entity/Entities/boar.hpp"
#include "Entity/Entities/deer.hpp"
#include "Entity/Entities/wolf.hpp"
#include "Entity/entity.hpp"
#include "Flowfield/flowfield.hpp"
#include "Map/map.hpp"
#include "Player/player.hpp"
#include "utils/behavior.hpp"
#include "utils/directions.hpp"
#include "utils/entityType.hpp"
#include "utils/state.hpp"

struct EntityInfo {
    float        x, y;
    bool         isAlive;
    bool         isAggressive;
    EntityType   type;
    Direction    direction;
    BehaviorType behavior;
};

class Game {
public:
    Game(int waterValue, int sizeValue, int typeValue);
    ~Game();
    Map map;

    void update(InputState input);

    auto getPlayerX() const -> int32_t
    {
        return player.getX();
    }

    auto getPlayerY() const -> int32_t
    {
        return player.getY();
    }

    int getScore() const { return player.getScore(); }

    std::vector<EntityInfo>       getEntitiesInfo() const;
    std::set<std::pair<int, int>> occupiedTiles;

    bool                               isKeyPressed(int keyCode) const;
    std::optional<std::pair<int, int>> getRandomPlacablePosition();

    bool isPlayerAlive();

    void setKeyState(int keyCode, bool pressed)
    {
        keyStates[keyCode] = pressed;
    }

    void clearKeyStates()
    {
        keyStates.clear();
    }

    Direction getPlayerDirection() const
    {
        return player.getDirection();
    }

    BehaviorType getPlayerBehavior() const
    {
        return player.getBehavior();
    }

private:
    FlowField flowField;
    Player    player;

    std::vector<Entity*> entities;
    void                 handlePlayerMovement(const InputState& inputState);

    std::unordered_map<int, bool> keyStates;

    void generateEntities(int countWolf, int countBoar, int countDeer);
    void placeEntityRandomly(Entity* entity);

    void updateFlowField();
    void updateEntities();

    bool isWalkableTile(int x, int y) const;
};
