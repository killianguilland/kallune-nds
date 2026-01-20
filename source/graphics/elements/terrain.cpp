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

u16 skyColor16 = RGB15(144 / 8, 216 / 8, 216 / 8) | BIT(15);

// 2. On la répète pour créer une valeur 32 bits (Pixel 1 | Pixel 2)
u32 skyColor32 = skyColor16 | (skyColor16 << 16);

const int cullingOffset = 1;

Terrain::Terrain()
{
    // Initialisation des pointeurs de buffers (Mode 5)
    // Bank A (128ko) et Bank C (128ko)
    this->frontBuffer          = (u16*)BG_GFX;
    this->backBuffer           = (u16*)0x06020000;
    this->currentWritingBuffer = this->backBuffer;
}

// On utilise 'inline' et on demande au compilateur d'optimiser à fond
__attribute__((always_inline)) inline void Terrain::renderSpanTile(const SpanTile& tile, int screenX, int screenY, bool needsClipping)
{
    const u32* offsets = tile.offsets;
    const u16* data    = tile.data;

    // 1. Clipping Vertical Hoisting (On calcule les bornes AVANT la boucle)
    int startL = 0;
    int endL   = 32;

    if (needsClipping)
    {
        if (screenY < 0)
            startL = -screenY;
        if (screenY + 32 > 192)
            endL = 192 - screenY;
        if (startL >= 32 || endL <= 0)
            return;
    }

    for (int l = startL; l < endL; l++)
    {
        int        drawY    = screenY + l;
        const u16* ptr      = &data[offsets[l]];
        int        numSpans = *ptr++;
        u16*       dstRow   = &this->currentWritingBuffer[drawY * 256];

        while (numSpans--)
        {
            int        spanX       = *ptr++;
            int        originalLen = *ptr++;
            const u16* src         = ptr;
            ptr += originalLen; // On avance déjà le pointeur pour le prochain span

            int xStart = screenX + spanX;

            if (needsClipping)
            {
                int len      = originalLen;
                int clipLeft = 0;
                if (xStart < 0)
                {
                    clipLeft = -xStart;
                    xStart   = 0;
                }
                int xEnd = xStart + (len - clipLeft);
                if (xEnd > 256)
                    len -= (xEnd - 256);
                len -= clipLeft;

                if (len <= 0)
                    continue;

                const u16* finalSrc = src + clipLeft;
                for (int i = 0; i < len; i++)
                    dstRow[xStart + i] = finalSrc[i];
            }
            else
            {
                // --- CHEMIN ULTRA RAPIDE ---
                // Ici, aucune vérification. On sait que ça rentre.
                // Optimisation possible : utiliser memcpy ou des copies u32
                for (int i = 0; i < originalLen; i++)
                {
                    dstRow[xStart + i] = src[i];
                }
            }
        }
    }
}

void Terrain::draw(const Map& mapgen, int playerX, int playerY)
{
    dmaFillWords(skyColor32, this->currentWritingBuffer, 256 * 192 * 2);

    const auto& map     = mapgen.getMap();
    int         mapSize = mapgen.getWidth();

    // Pré-calculer les scrolls car ils ne changent pas dans la boucle
    const int fineScrollX = (playerX - playerY) * 16 + 16 - 128;
    const int fineScrollY = (playerX + playerY) * 8 - 96;

    for (int row = -12 - cullingOffset; row < 10 + 2 + cullingOffset; row++)
    {
        const int  rowHalf = (row >> 1);
        const bool isEven  = ((row & 1) == 0);
        const int  maxCol  = isEven ? 4 + cullingOffset : 5 + cullingOffset;

        for (int col = -3 - cullingOffset; col < maxCol; col++)
        {
            int x = playerX + rowHalf + col;
            int y = playerY + (row - rowHalf) - col;

            if (x < 0 || y < 0 || x >= mapSize || y >= mapSize)
                continue;

            int sX = (x - y) * 16 - fineScrollX;
            int sY = (x + y) * 8 - fineScrollY;

            const MapType tileType = mapgen.at(x, y);
            const auto&   typeData = terrainTilesTable[static_cast<int>(tileType)];

            // Start with the default tile
            const SpanTile* tileToDraw = typeData.defaultTile;

            // ONLY run the expensive neighbor logic if this tile type supports it
            if (typeData.isAutotiled)
            {
                uint8_t mask = 0;
                // Fast 1D neighbor checks
                if (y > 0 && mapgen.at(x, y - 1) != tileType)
                    mask |= 1;
                if (y < mapSize - 1 && mapgen.at(x, y + 1) != tileType)
                    mask |= 2;
                if (x > 0 && mapgen.at(x - 1, y) != tileType)
                    mask |= 4;
                if (x < mapSize - 1 && mapgen.at(x + 1, y) != tileType)
                    mask |= 8;

                tileToDraw = typeData.variations[mask];
            }

            sY -= typeData.heightOffset;

            // Now call your render function with tileToDraw
            if (sX >= 0 && sX <= 224 && sY >= 0 && sY <= 160)
            {
                renderSpanTile(*tileToDraw, sX, sY, false);
            }
            else
            {
                renderSpanTile(*tileToDraw, sX, sY, true);
            }
        }
    }
}

void Terrain::swapBuffers()
{
    // Changer la page affichée par le moteur BG3 de la DS
    if (this->currentWritingBuffer == this->backBuffer)
    {
        // On affiche la Bank C (Back)
        REG_BG3CNT                 = (REG_BG3CNT & ~0x1F00) | (8 << 8);
        this->currentWritingBuffer = this->frontBuffer;
    }
    else
    {
        // On affiche la Bank A (Front)
        REG_BG3CNT                 = (REG_BG3CNT & ~0x1F00) | (0 << 8);
        this->currentWritingBuffer = this->backBuffer;
    }
}