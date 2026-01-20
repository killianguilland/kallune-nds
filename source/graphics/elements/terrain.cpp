#include "terrain.hpp"
#include <array>

// Tableau de correspondance MapType -> Span-List
static const std::array<SpanTile, 6> terrainTilesTable = {{
    {tile_104_offsets, tile_104_data}, // Water
    {tile_061_offsets, tile_061_data}, // Sand
    {tile_024_offsets, tile_024_data}, // Grass
    {tile_000_offsets, tile_000_data}, // Wall
    {tile_104_offsets, tile_104_data}, // Flower
    {tile_025_offsets, tile_025_data}, // Solid Wall
}};

static const std::array<int, 6> terrainTilesOffset = {{
    0,  // Water
    0,  // Sand
    0,  // Grass
    5,  // Wall
    0,  // Flower
    10, // Solid Wall
}};

u16 skyColor16 = RGB15(144 / 8, 216 / 8, 216 / 8) | BIT(15);

// 2. On la répète pour créer une valeur 32 bits (Pixel 1 | Pixel 2)
u32 skyColor32 = skyColor16 | (skyColor16 << 16);

const int cullingOffset = 2;

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

    for (int row = -12 - cullingOffset; row < 10 + cullingOffset; row++)
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

            const int tileType = static_cast<int>(map[x][y]);

            sY -= terrainTilesOffset[tileType];

            if (sX <= -32 || sX >= 256 || sY <= -32 || sY >= 192)
                continue;

            const SpanTile& tile = terrainTilesTable[tileType];

            // --- DETECTION DU FAST PATH ---
            // Si la tuile est entre X[0-224] et Y[0-160], elle ne nécessite aucun clipping
            if (sX >= 0 && sX <= 224 && sY >= 0 && sY <= 160)
            {
                renderSpanTile(tile, sX, sY, false); // Version sans clipping
            }
            else
            {
                renderSpanTile(tile, sX, sY, true); // Version avec clipping
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