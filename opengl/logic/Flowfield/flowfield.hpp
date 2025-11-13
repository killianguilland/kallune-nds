#pragma once

#include <vector>
#include <queue>
#include "../Map/map.hpp"

class FlowField {
public:
    FlowField(int width, int height);

    void computeFlowField(int targetX, int targetY, Map& map);
    void getDirectionAt(int x, int y, float& outX, float& outY) const;

private:
    int width, height;

    struct Cell {
        float cost = 1e9f;
        int fromX = -1;
        int fromY = -1;
    };

    std::vector<std::vector<Cell>> field;
};
