#pragma once
#include <vector>
#include "graphics/elements/sprite.hpp"
#include "logic/game.hpp"
#include "utils/map.hpp"
#include "glbasimac/glbi_engine.hpp"

#include <PerlinNoise.hpp>

using namespace glbasimac;
extern GLBI_Engine GameEngine;

class MapDisplay
{
public:
    MapDisplay();
    ~MapDisplay() = default;

    void update(double deltaTime, Game game);
    void draw(double deltaTime, Game game);

private:
    std::vector<Sprite *> tileset;
    std::vector<std::vector<double>> noiseMap;
};