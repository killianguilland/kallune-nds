#pragma once

#include <nds.h>
#include <playerIcon.h>
#include <logic/Map/map.hpp>
#include <logic/game.hpp>

class Minimap {
private:
    u16* fullMapBuffer{nullptr};
    u16* playerSpriteGfx{nullptr};
    void safePlot(int x, int y, u16 color);
    int  mapWidth{};

public:
    Minimap(const Map& mapgen);
    void draw(int playerX, int playerY);
    ~Minimap();

    // Minimap(const Minimap&) = delete;
    // auto operator=(const Minimap&) -> Minimap& = delete;
    // Minimap(Minimap&&) = delete;
    // auto operator=(Minimap&&) -> Minimap& = delete;
};