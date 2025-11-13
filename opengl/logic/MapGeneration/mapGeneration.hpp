#pragma once

#include <iostream>
#include <vector>
#include "utils/map.hpp"
using namespace std;

class MapGeneration
{
public:
    MapGeneration(unsigned int width, unsigned int height);

    int getValue(unsigned int x, unsigned int y);
    vector<vector<MapType>> getMap();

    void setWidth(unsigned int width);
    unsigned int getWidth();
    void setHeight(unsigned int height);
    unsigned int getHeight();

    void generate(int chanceToStartAlive = 40, unsigned int numberOfSteps = 6, unsigned int starvationLimit = 2, unsigned int overpopLimit = 3, unsigned int birthNumber = 3);

    void postProcess();

private:
    vector<vector<int>> map;
    unsigned int width;
    unsigned int height;

    void printMap();
    void step();
    int countNeighbors(int x, int y, int dist, vector<int> valueToSearch);
};
