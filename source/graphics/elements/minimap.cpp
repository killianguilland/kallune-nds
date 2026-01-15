#include "minimap.hpp"
#include <array>
#include <cstdio>

const int width = 256; // Mode 5 width

constexpr std::array<u16, 6> initColorTable()
{
    std::array<u16, 6> table{};
    table[(int)MapType::WATER]      = RGB15(86 / 8, 192 / 8, 214 / 8) | BIT(15);
    table[(int)MapType::SAND]       = RGB15(123 / 8, 152 / 8, 150 / 8) | BIT(15);
    table[(int)MapType::GRASS]      = RGB15(159 / 8, 203 / 8, 65 / 8) | BIT(15);
    table[(int)MapType::WALL]       = RGB15(182 / 8, 116 / 8, 81 / 8) | BIT(15);
    table[(int)MapType::SOLID_WALL] = RGB15(130 / 8, 69 / 8, 63 / 8) | BIT(15);
    table[(int)MapType::FLOWER]     = RGB15(255 / 8, 243 / 8, 10 / 8) | BIT(15);
    return table;
}

constexpr auto MapColorTable = initColorTable();

const int BUF_W = 512;
const int BUF_H = 256;

void Minimap::safePlot(int x, int y, u16 color)
{
    if (x >= 0 && x < BUF_W && y >= 0 && y < BUF_H)
    {
        fullMapBuffer[y * BUF_W + x] = color;
    }
    else
    {
        fprintf(stderr, "SAFEPLOT PREVENTED SEGFAULT at x %d y %d", x, y);
    }
}

Minimap::Minimap(const Map& mapgen)
{
    playerSpriteGfx = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_16Color);

    // 2. Copier les tuiles (Tiles) vers la VRAM allouée
    dmaCopy(playerIconTiles, playerSpriteGfx, playerIconTilesLen);

    // 3. Copier la palette vers la mémoire palette des sprites du moteur SUB
    // On la met à l'index 0 (la première palette de 16 couleurs)
    dmaCopy(playerIconPal, SPRITE_PALETTE_SUB, playerIconPalLen);

    // 1. Allouer la mémoire en RAM (256 Ko pour un buffer 512x256)
    fullMapBuffer = (u16*)malloc(BUF_W * BUF_H * sizeof(u16));

    u16 bgColor   = RGB15(168 / 8, 112 / 8, 80 / 8) | BIT(15);
    u16 lineColor = RGB15(152 / 8, 96 / 8, 72 / 8) | BIT(15);

    memset(fullMapBuffer, 0, BUF_W * BUF_H * sizeof(u16));

    for (int x = 0; x < BUF_W; x++)
    {
        for (int y = 0; y < BUF_H; y++)
        {
            safePlot(x, y, bgColor);
            // fullMapBuffer[y * BUF_W + x] = bgColor;
        }
    }

    const int tileSize = 16;
    const int halfSize = 7;

    for (int xOffset = 0; xOffset < BUF_W; xOffset += tileSize * 2)
    {
        for (int yOffset = 0; yOffset < BUF_H; yOffset += tileSize)
        {
            for (int i = 0; i <= halfSize; i++)
            {
                int xTopLeft = xOffset + (i * 2);
                int yTopLeft = yOffset + halfSize - i;

                safePlot(xTopLeft, yTopLeft, lineColor);
                safePlot(xTopLeft + 1, yTopLeft, lineColor);

                // fullMapBuffer[yTopLeft * BUF_W + xTopLeft]     = lineColor;
                // fullMapBuffer[yTopLeft * BUF_W + xTopLeft + 1] = lineColor;

                int xTopRight = xOffset + ((halfSize + i) * 2) + 2;
                int yTopRight = yOffset + i;

                safePlot(xTopRight, yTopRight, lineColor);
                safePlot(xTopRight + 1, yTopRight, lineColor);

                // fullMapBuffer[yTopRight * BUF_W + xTopRight]     = lineColor;
                // fullMapBuffer[yTopRight * BUF_W + xTopRight + 1] = lineColor;

                int xBottomLeft = xOffset + (i * 2);
                int yBottomLeft = yOffset + halfSize + 1 + i;

                safePlot(xBottomLeft, yBottomLeft, lineColor);
                safePlot(xBottomLeft + 1, yBottomLeft, lineColor);

                // fullMapBuffer[yBottomLeft * BUF_W + xBottomLeft]     = lineColor;
                // fullMapBuffer[yBottomLeft * BUF_W + xBottomLeft + 1] = lineColor;

                int xBottomRight = xOffset + ((halfSize + i) * 2) + 2;
                int yBottomRight = yOffset + tileSize - 1 - i;

                safePlot(xBottomRight, yBottomRight, lineColor);
                safePlot(xBottomRight + 1, yBottomRight, lineColor);

                // fullMapBuffer[yBottomRight * BUF_W + xBottomRight]     = lineColor;
                // fullMapBuffer[yBottomRight * BUF_W + xBottomRight + 1] = lineColor;
            }
        }
    }

    const auto& map      = mapgen.getMap();
    int         mapWidth = mapgen.getWidth();

    // On centre le dessin dans notre buffer de 512x256
    int offsetX = BUF_W / 2 - 1;
    int offsetY = BUF_H / 2 - mapWidth;

    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapWidth; y++)
        {
            int isoX = (x - y) * 2 + offsetX;
            int isoY = (x + y) + offsetY;

            u16 color = MapColorTable[(int)map[x][y]];

            // Sécurité : On vérifie les bornes avant d'écrire
            if (isoX > 0 && isoX < BUF_W - 2 && isoY > 0 && isoY < BUF_H - 2)
            {
                // Ligne 1
                safePlot(isoX, isoY, color);
                safePlot(isoX + 1, isoY, color);
                // fullMapBuffer[isoY * BUF_W + isoX]     = color;
                // fullMapBuffer[isoY * BUF_W + isoX + 1] = color;

                // Ligne 2 (le bloc de 4 pixels de large)
                int isoY2 = isoY + 1;

                safePlot(isoX - 1, isoY2, color);
                safePlot(isoX, isoY2, color);
                safePlot(isoX + 1, isoY2, color);
                safePlot(isoX + 2, isoY2, color);
                // fullMapBuffer[isoY2 * BUF_W + isoX - 1] = color;
                // fullMapBuffer[isoY2 * BUF_W + isoX]     = color;
                // fullMapBuffer[isoY2 * BUF_W + isoX + 1] = color;
                // fullMapBuffer[isoY2 * BUF_W + isoX + 2] = color;
            }
        }
    }
}

void Minimap::draw(int playerX, int playerY)
{
    // 0. SÉCURITÉ : On ne fait rien si le buffer n'existe pas
    if (this->fullMapBuffer == nullptr)
        return;

    u16* vramBase = (u16*)BG_BMP_RAM_SUB(0);

    // 1. CALCUL DE LA FENÊTRE (Scrolling)
    int viewX = (playerX - playerY) * 2 + (BUF_W / 2) - 128;
    int viewY = (playerX + playerY) + 20 - 96;

    // Bornes de l'écran (sous-fenêtre de la minimap)
    const int minY = 16;
    const int maxY = 164; // 192 - 28

    // --- CLAMPING STRICT (Anti-Crash) ---

    // viewX doit être entre 0 et (LargeurBuffer - LargeurEcran)
    if (viewX < 0)
        viewX = 0;
    if (viewX > (BUF_W - 256))
        viewX = (BUF_W - 256);

    // IMPORTANT : On force viewX à être pair pour l'alignement mémoire (DMA)
    viewX &= ~1;

    // viewY doit être tel que (viewY + l) ne sorte jamais de [0, BUF_H - 1]
    // La ligne la plus basse lue sera (viewY + maxY - 1)
    if (viewY < -minY)
        viewY = -minY;
    if (viewY > (BUF_H - maxY))
        viewY = (BUF_H - maxY);

    // 2. TRANSFERT DMA LIGNE PAR LIGNE
    for (int l = minY; l < maxY; l++)
    {
        int bufferLine = viewY + l;

        // Double vérification de l'index de ligne
        if (bufferLine >= 0 && bufferLine < BUF_H)
        {
            // Calcul de l'adresse source
            u16* src = &fullMapBuffer[bufferLine * BUF_W + viewX];

            // Calcul de l'adresse destination (VRAM écran du bas)
            u16* dst = &vramBase[l * 256];

            // Copie de 256 pixels (512 octets)
            dmaCopy(src, dst, 512);
        }
    }

    // 3. MISE À JOUR DU SPRITE JOUEUR
    if (this->playerSpriteGfx != nullptr)
    {
        int absX = (playerX - playerY) * 2 + (BUF_W / 2);
        int absY = (playerX + playerY) + 20;

        int screenX = absX - viewX;
        int screenY = absY - viewY;

        // On cache le sprite s'il sort de la zone de la minimap
        bool hide = (screenY < minY || screenY >= maxY || screenX < 0 || screenX >= 256);

        oamSet(&oamSub,
               2,           // ID du sprite
               screenX - 4, // Centrage
               screenY - 4, // Centrage
               0,           // Priorité
               0,           // Palette
               SpriteSize_8x8, SpriteColorFormat_16Color, playerSpriteGfx, -1, false, hide, false, false, false);

        oamUpdate(&oamSub);
    }
}

Minimap::~Minimap()
{
    if (this->fullMapBuffer != nullptr)
    {
        fprintf(stderr, "clearing minimap");
        free(this->fullMapBuffer);
        this->fullMapBuffer = nullptr;
    }
}