#include "terrain.hpp"
#include <array>

// Tableau de correspondance MapType -> Span-List
static const std::array<SpanTile, 6> terrainTilesTable = {{
    {tile_104_offsets, tile_104_data}, // Water
    {tile_061_offsets, tile_061_data}, // Sand
    {tile_037_offsets, tile_037_data}, // Grass
    {tile_000_offsets, tile_000_data}, // Wall
    {tile_104_offsets, tile_104_data}, // Flower
    {tile_025_offsets, tile_025_data}, // Solid Wall
}};

static const std::array<int, 6> terrainTilesOffset = {{
    0,  // Water
    0,  // Sand
    0,  // Grass
    16, // Wall
    0,  // Flower
    24, // Solid Wall
}};

Terrain::Terrain()
{
    // Initialisation des pointeurs de buffers (Mode 5)
    // Bank A (128ko) et Bank C (128ko)
    this->frontBuffer          = (u16*)BG_GFX;
    this->backBuffer           = (u16*)0x06020000;
    this->currentWritingBuffer = this->backBuffer;
}

void Terrain::renderSpanTile(const SpanTile& tile, int screenX, int screenY)
{
    const u32* offsets = tile.offsets;
    const u16* data    = tile.data;

    for (int l = 0; l < 32; l++)
    {
        int drawY = screenY + l;
        if (drawY < 0 || drawY >= 192)
            continue;

        const u16* ptr      = &data[offsets[l]];
        int        numSpans = *ptr++;
        u16*       dstRow   = &this->currentWritingBuffer[drawY * 256];

        while (numSpans--)
        {
            int spanX       = *ptr++;
            int originalLen = *ptr++;
            int len         = originalLen;

            int xStart = screenX + spanX;
            int xEnd   = xStart + len;

            int clipLeft = 0;
            if (xStart < 0)
            {
                clipLeft = -xStart;
                xStart   = 0;
            }
            if (xEnd > 256)
            {
                len -= (xEnd - 256);
            }
            len -= clipLeft;

            if (len > 0)
            {
                const u16* src = ptr + clipLeft;
                // On retire le debug color-coding pour la performance
                for (int i = 0; i < len; i++)
                    dstRow[xStart + i] = src[i];
            }
            ptr += originalLen;
        }
    }
}

void Terrain::draw(const Map& mapgen, int playerX, int playerY)
{
    // Effacement DMA (Rapide)
    dmaFillWords(RGB15(0, 0, 0) | BIT(15), this->currentWritingBuffer, 256 * 192 * 2);

    const auto& map     = mapgen.getMap();
    int         mapSize = mapgen.getWidth();

    // On calcule le décalage de scroll fin (0 à 31 pixels)
    int fineScrollX = (playerX - playerY) * 16;
    int fineScrollY = (playerX + playerY) * 8;

    // Cette boucle parcourt l'écran comme une grille de tuiles 32x16
    // On part de camX/camY et on s'étend pour couvrir 256x192
    for (int row = -12; row < 10; row++) // Vertical
    {
        // On détermine si la ligne est paire ou impaire
        bool isEven = ((row & 1) == 0);

        // Si on veut ajouter juste une demi-colonne,
        // on n'autorise l'itération supplémentaire que pour les lignes paires (ou impaires)
        int maxCol = isEven ? 4 : 5;

        for (int col = -3; col < maxCol; col++) // Horizontal
        {
            const int rowHalf = (row >> 1);
            // Formule magique pour transformer row/col d'écran en X/Y de map
            // sans AUCUN doublon de tuile
            int x = playerX + rowHalf + col;
            int y = playerY + (row - rowHalf) - col;

            if (x < 0 || y < 0 || x >= mapSize || y >= mapSize)
                continue;

            // Position écran relative à la caméra
            int sX = (x - y) * 16 - fineScrollX + 128 - 16;
            int sY = (x + y) * 8 - fineScrollY + 96;

            const int tileType = static_cast<int>(map[x][y]);

            // sY -= terrainTilesOffset[tileType];
            const SpanTile& tile = terrainTilesTable[tileType];

            renderSpanTile(tile, sX, sY);
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