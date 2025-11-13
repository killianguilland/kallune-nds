#include "flowfield.hpp"
#include <cmath>
#include <limits>

FlowField::FlowField(int width, int height)
    : width(width), height(height), field(width, std::vector<Cell>(height)) {}

void FlowField::computeFlowField(int targetX, int targetY, Map& map) {
    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
            field[x][y] = Cell();

    using Node = std::pair<float, std::pair<int, int>>; 
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open;

    field[targetX][targetY].cost = 0.0f;
    open.push({0.0f, {targetX, targetY}});

    auto inBounds = [&](int x, int y) {
        return x >= 0 && x < width && y >= 0 && y < height;
    };

    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    auto speedMap = map.getSpeedMap();

    while (!open.empty()) {
        auto [currCost, pos] = open.top();
        int cx = pos.first;
        int cy = pos.second;
        open.pop();

        if (currCost > field[cx][cy].cost) continue;

        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            if (!inBounds(nx, ny)) continue;

            float speed = speedMap[nx][ny]; 
            if (speed <= 0.0f) continue; 

            float moveCost = 1.0f / speed;
            float newCost = currCost + moveCost;

            if (newCost < field[nx][ny].cost) {
                field[nx][ny].cost = newCost;
                field[nx][ny].fromX = cx;
                field[nx][ny].fromY = cy;
                open.push({newCost, {nx, ny}});
            }
        }
    }
}

void FlowField::getDirectionAt(int x, int y, float& outX, float& outY) const {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        outX = 0.0f;
        outY = 0.0f;
        return;
    }

    const Cell& cell = field[x][y];
    if (cell.fromX == -1 || cell.fromY == -1) { 
        outX = 0.0f;
        outY = 0.0f;
        return;
    }

    outX = static_cast<float>(cell.fromX - x);
    outY = static_cast<float>(cell.fromY - y);
}
