#pragma once

#include <nds.h>
#include <tile_000.h>
#include <tile_024.h>
#include <tile_025.h>
#include <tile_037.h>
#include <tile_061.h>
#include <tile_104.h>
#include <logic/game.hpp>
#include <vector>

struct SpanTile {
    const u32* offsets;
    const u16* data;
};

class Terrain {
private:
    u16*        frontBuffer;
    u16*        backBuffer;
    u16*        currentWritingBuffer;
    inline void renderSpanTile(const SpanTile& tile, int screenX, int screenY, bool needsClipping);

public:
    Terrain();
    // posX et posY correspondent à la position de la caméra dans le monde (0 à 8192)
    void draw(const Map& mapgen, int playerX, int playerY);
    void swapBuffers();
    ~Terrain() = default;
};