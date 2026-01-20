#pragma once

#include <iostream>
#include <vector>
#include "../MapGeneration/mapGeneration.hpp"
#include "utils/mapUtils.hpp"
using namespace std;

class Map {
public:
    Map(int waterValue, int sizeValue, int typeValue);

    std::vector<float> getSpeedMap() const;

    float getSpeed(MapType type) const;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    [[nodiscard]] MapType at(int x, int y) const { return map[y * width + x]; }
    MapType&              at(int x, int y) { return map[y * width + x]; }

    void                        removeFlower(int tileX, int tileY);
    const std::vector<MapType>& getMap() const { return map; }
    bool                        isWalkable(int tileX, int tileY);

    void changeTile(int tileX, int tileY, MapType newType);
    // void init();

private:
    std::vector<MapType> map;
    int                  width;
    int                  height;

public:
    MapGeneration generator;
};
