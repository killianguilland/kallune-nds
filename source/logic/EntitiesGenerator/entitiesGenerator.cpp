#include "entitiesGenerator.hpp"

EntitiesGenerator::EntitiesGenerator(FlowField *flowField, Player *player)
    : flowField(flowField), player(player)
{
}

void EntitiesGenerator::update(float deltaTime)
{
    for (auto it = entities.begin(); it != entities.end();)
    {
        (*it)->update(deltaTime);

        if (!(*it)->isAlive())
        {
            delete *it;
            it = entities.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

const std::vector<Entity*>& EntitiesGenerator::getEntities() const
{
    return entities;
}

EntitiesGenerator::~EntitiesGenerator()
{
    for (auto entity : entities)
    {
        delete entity;
    }
}