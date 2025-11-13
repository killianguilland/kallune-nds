#pragma once

#include <iostream>
#include <vector>
#include "../MapGeneration/mapGeneration.hpp"
#include "utils/map.hpp"
using namespace std;

class Map
{
public:
    Map();

    vector<vector<float>> getSpeedMap() const;

    float getSpeed(MapType type) const;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void removeFlower(int tileX, int tileY);
    const vector<vector<MapType>>& getMap() const { return map; }
    bool isWalkable(int tileX, int tileY);

    void changeTile(int tileX, int tileY, MapType newType);
private:
    vector<vector<MapType>> map;
    int width;
    int height;

public:
    MapGeneration generator;
};
