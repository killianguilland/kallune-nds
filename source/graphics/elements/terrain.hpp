#pragma once

#include <nds.h>
#include <tile_000.h>
#include <tile_001.h>
#include <tile_002.h>
#include <tile_004.h>
#include <tile_005.h>
#include <tile_008.h>
#include <tile_009.h>
#include <tile_010.h>
#include <tile_024.h>
#include <tile_025.h>
#include <tile_037.h>
#include <tile_061.h>
#include <tile_104.h>
#include <tile_105.h>
#include <tile_106.h>
#include <tile_107.h>
#include <tile_108.h>
#include <tile_109.h>
#include <tile_110.h>
#include <tile_111.h>
#include <tile_112.h>
#include <tile_113.h>
#include <logic/game.hpp>
#include <vector>

struct SpanTile {
    const u32* offsets;
    const u16* data;
};

struct TerrainVariation {
    const SpanTile*  defaultTile; // Used if isAutotiled is false
    const SpanTile** variations;  // Pointer to an array of 16 pointers (or nullptr)
    int              heightOffset;
    bool             isAutotiled; // THE KEY: Skip logic if false
};

class Terrain {
private:
    u16*        frontBuffer;
    u16*        backBuffer;
    u16*        currentWritingBuffer;
    inline void renderSpanTile(const SpanTile& tile, int screenX, int screenY, bool needsClipping);
    int         lastCamX    = -9999;
    int         lastCamY    = -9999;
    bool        firstRender = true;

    const int BUF_MASK = 255;

public:
    Terrain();
    // posX et posY correspondent à la position de la caméra dans le monde (0 à 8192)
    void draw(const Map& mapgen, int32_t playerX_fp, int32_t playerY_fp);
    void renderFullArea(const Map& mapgen, int32_t pX_fp, int32_t pY_fp);
    void injectBorders(const Map& mapgen, int32_t pX_fp, int32_t pY_fp, int camX, int camY, int dx, int dy);
    void renderSpanTileWrapped(const SpanTile& tile, int worldX, int worldY);
    ~Terrain() = default;
};