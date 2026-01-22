#include "terrain.hpp"
#include <array>

// Base Tiles
static const SpanTile tile104 = {tile_104_offsets, tile_104_data}; // Normal
static const SpanTile tile105 = {tile_105_offsets, tile_105_data}; // Top
static const SpanTile tile106 = {tile_106_offsets, tile_106_data}; // Left
static const SpanTile tile107 = {tile_107_offsets, tile_107_data}; // Right
static const SpanTile tile108 = {tile_108_offsets, tile_108_data}; // Bottom
static const SpanTile tile109 = {tile_109_offsets, tile_109_data}; // Top-Left
static const SpanTile tile110 = {tile_110_offsets, tile_110_data}; // Bottom-Right
static const SpanTile tile111 = {tile_111_offsets, tile_111_data}; // Bottom-Left
static const SpanTile tile112 = {tile_112_offsets, tile_112_data}; // Top-Right
static const SpanTile tile113 = {tile_113_offsets, tile_113_data}; // Surrounded

static const SpanTile* waterVariants[16] = {
    &tile104, // 0000: Normal
    &tile105, // 0001: Top
    &tile108, // 0010: Bottom
    &tile104, // 0011: Top + Bottom (Default to normal)
    &tile106, // 0100: Left
    &tile109, // 0101: Top + Left (TOP_LEFT)
    &tile111, // 0110: Bottom + Left (BOTTOM_LEFT)
    &tile106, // 0111: Top + Bottom + Left (Default to Left)
    &tile107, // 1000: Right
    &tile112, // 1001: Top + Right (TOP_RIGHT)
    &tile110, // 1010: Bottom + Right (BOTTOM_RIGHT)
    &tile107, // 1011: Top + Bottom + Right (Default to Right)
    &tile104, // 1100: Left + Right (Default to normal)
    &tile105, // 1101: Top + Left + Right (Default to Top)
    &tile108, // 1110: Bottom + Left + Right (Default to Bottom)
    &tile113  // 1111: Surrounded
};

// Wall Base Tiles
static const SpanTile tile000 = {tile_000_offsets, tile_000_data}; // BOTTOM_LEFT
static const SpanTile tile002 = {tile_002_offsets, tile_002_data}; // TOP_RIGHT
static const SpanTile tile004 = {tile_004_offsets, tile_004_data}; // BOTTOM_RIGHT
static const SpanTile tile005 = {tile_005_offsets, tile_005_data}; // TOP & TOP_LEFT
static const SpanTile tile008 = {tile_008_offsets, tile_008_data}; // SURROUNDED
static const SpanTile tile009 = {tile_009_offsets, tile_009_data}; // NORMAL, BOTTOM, LEFT, RIGHT

static const SpanTile* wallVariants[16] = {
    &tile009, // 0000: NORMAL
    &tile005, // 0001: TOP
    &tile009, // 0010: BOTTOM (Uses ID 9)
    &tile009, // 0011: Top + Bottom (Default to 9)
    &tile009, // 0100: LEFT (Uses ID 9)
    &tile005, // 0101: TOP_LEFT (Uses ID 5)
    &tile000, // 0110: BOTTOM_LEFT (Uses ID 0)
    &tile009, // 0111: Top + Bottom + Left (Default to 9)
    &tile009, // 1000: RIGHT (Uses ID 9)
    &tile002, // 1001: TOP_RIGHT (Uses ID 2)
    &tile004, // 1010: BOTTOM_RIGHT (Uses ID 4)
    &tile009, // 1011: Top + Bottom + Right (Default to 9)
    &tile009, // 1100: Left + Right (Default to 9)
    &tile005, // 1101: Top + Left + Right (Default to 5)
    &tile009, // 1110: Bottom + Left + Right (Default to 9)
    &tile008  // 1111: SURROUNDED (Uses ID 8)
};

static const SpanTile waterBase = {tile_104_offsets, tile_104_data};
static const SpanTile sandBase  = {tile_061_offsets, tile_061_data};
static const SpanTile grassBase = {tile_024_offsets, tile_024_data};
static const SpanTile wallBase  = {tile_000_offsets, tile_000_data};
static const SpanTile solidBase = {tile_025_offsets, tile_025_data};

static const std::array<TerrainVariation, 6> terrainTilesTable = {{
    {&waterBase, waterVariants, 0, true}, // Water
    {&sandBase, nullptr, 0, false},       // Sand
    {&grassBase, nullptr, 0, false},      // Grass
    {&wallBase, wallVariants, 12, true},  // Wall
    {&waterBase, nullptr, 0, false},      // Flower
    {&solidBase, nullptr, 15, false}      // Solid Wall
}};

const int        cullingOffset = 1;
static const u16 skyColor16    = RGB15(144 / 8, 216 / 8, 216 / 8) | BIT(15);
static const u32 skyColor32    = skyColor16 | (skyColor16 << 16);

Terrain::Terrain()
{
    this->firstRender = true;
    this->lastCamX    = -9999;
    this->lastCamY    = -9999;
}

ITCM_CODE void Terrain::renderSpanTileWrapped(const SpanTile& tile, int worldX, int worldY)
{
    const u16* data    = tile.data;
    const u32* offsets = tile.offsets;

    for (int l = 0; l < 32; l++)
    {
        int        targetY = (worldY + l) & 255;
        const u16* ptr     = &data[offsets[l]];

        int numSpans = *ptr++;

        u16* vramRow = (u16*)BG_BMP_RAM(0) + (targetY << 8);

        while (numSpans--)
        {
            int        spanX = *ptr++;
            int        len   = *ptr++;
            const u16* src   = ptr;
            ptr += len;
            if (len & 1)

                for (int i = 0; i < len; i++)
                {
                    int targetX      = (worldX + spanX + i) & 255;
                    vramRow[targetX] = src[i];
                }
        }
    }
}

void Terrain::draw(const Map& mapgen, int32_t playerX_fp, int32_t playerY_fp)
{
    int camX = (((playerX_fp - playerY_fp) * 16) >> 8) - 128;
    int camY = (((playerX_fp + playerY_fp) * 8) >> 8) - 96;

    REG_BG3X = camX << 8;
    REG_BG3Y = camY << 8;

    if (firstRender)
    {
        dmaFillWords(skyColor32, (void*)BG_BMP_RAM(0), 256 * 256 * 2);

        renderFullArea(mapgen, playerX_fp, playerY_fp);
        firstRender = false;
        lastCamX    = camX;
        lastCamY    = camY;
        return;
    }

    int dx = camX - lastCamX;
    int dy = camY - lastCamY;

    if (std::abs(dx) >= 4 || std::abs(dy) >= 4)
    {
        injectBorders(mapgen, playerX_fp, playerY_fp, camX, camY, dx, dy);
        lastCamX = camX;
        lastCamY = camY;
    }
}

// --- INJECTION DES BORDS ---
void Terrain::injectBorders(const Map& mapgen, int32_t pX_fp, int32_t pY_fp, int camX, int camY, int dx, int dy)
{
    int pX      = pX_fp >> 8;
    int pY      = pY_fp >> 8;
    int mapSize = mapgen.getWidth();

    for (int row = -14; row < 14; row++)
    {
        for (int col = -6; col < 8; col++)
        {
            bool isEdge = false;
            if (dx > 0 && col >= 4)
                isEdge = true;
            if (dx < 0 && col <= -4)
                isEdge = true;
            if (dy > 0 && row >= 10)
                isEdge = true;
            if (dy < 0 && row <= -12)
                isEdge = true;

            if (!isEdge)
                continue;

            int x = pX + (row >> 1) + col;
            int y = pY + (row - (row >> 1)) - col;

            if (x < 0 || y < 0 || x >= mapSize || y >= mapSize)
                continue;

            const auto&     typeData   = terrainTilesTable[static_cast<int>(mapgen.at(x, y))];
            const SpanTile* tileToDraw = typeData.defaultTile;

            if (typeData.isAutotiled)
            {
                uint8_t mask = 0;
                if (y > 0 && mapgen.at(x, y - 1) != mapgen.at(x, y))
                    mask |= 1;
                if (y < mapSize - 1 && mapgen.at(x, y + 1) != mapgen.at(x, y))
                    mask |= 2;
                if (x > 0 && mapgen.at(x - 1, y) != mapgen.at(x, y))
                    mask |= 4;
                if (x < mapSize - 1 && mapgen.at(x + 1, y) != mapgen.at(x, y))
                    mask |= 8;
                tileToDraw = typeData.variations[mask];
            }

            int sX = (x - y) * 16;
            int sY = (x + y) * 8 - typeData.heightOffset;

            renderSpanTileWrapped(*tileToDraw, sX, sY);
        }
    }
}

void Terrain::renderFullArea(const Map& mapgen, int32_t pX_fp, int32_t pY_fp)
{
    int pX = pX_fp >> 8;
    int pY = pY_fp >> 8;

    for (int row = -11; row < 11; row++)
    {
        for (int col = -5; col < 6; col++)
        {
            int x = pX + (row >> 1) + col;
            int y = pY + (row - (row >> 1)) - col;

            if (x < 0 || y < 0 || x >= mapgen.getWidth() || y >= mapgen.getHeight())
                continue;

            const auto& typeData = terrainTilesTable[static_cast<int>(mapgen.at(x, y))];

            int sX = (x - y) * 16;
            int sY = (x + y) * 8 - typeData.heightOffset;

            renderSpanTileWrapped(*typeData.defaultTile, sX, sY);
        }
    }
}