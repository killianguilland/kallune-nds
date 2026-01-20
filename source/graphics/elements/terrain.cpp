#include "terrain.hpp"
#include <array>


Terrain::Terrain()
{
    // Initialisation des pointeurs de buffers (Mode 5)
    // Bank A (128ko) et Bank C (128ko)
    this->frontBuffer          = (u16*)BG_GFX;
    this->backBuffer           = (u16*)0x06020000;
    this->currentWritingBuffer = this->backBuffer;
}

void Terrain::draw(const Map& mapgen, int camX, int camY)
{
    // Effacement rapide avec DMA
    dmaFillWords(RGB15(0, 0, 0) | BIT(15), this->currentWritingBuffer, 256 * 192 * 2);

    const std::vector<std::vector<MapType>>& map     = mapgen.getMap();
    int                                      mapSize = mapgen.getWidth();

    int isoScrollX = (camX - camY) * 16;
    int isoScrollY = (camX + camY) * 8;

    // Culling par grille (Losange)
    int range       = 10;
    int startX_grid = camX - range;
    int endX_grid   = camX + range;
    int startY_grid = camY - range;
    int endY_grid   = camY + range;

    // Bornes de sécurité pour la grille
    if (startX_grid < 0)
        startX_grid = 0;
    if (startY_grid < 0)
        startY_grid = 0;
    if (endX_grid > mapSize)
        endX_grid = mapSize;
    if (endY_grid > mapSize)
        endY_grid = mapSize;

    for (int y = startY_grid; y < endY_grid; y++)
    {
        for (int x = startX_grid; x < endX_grid; x++)
        {
            // Position écran relative au centre
            int screenX = (x - y) * 16 - isoScrollX + 128;
            int screenY = (x + y) * 8 - isoScrollY + 96 - 16;

            // Culling tuile entière
            if (screenX < -32 || screenX >= 256 || screenY < -32 || screenY >= 192)
                continue;

            // Choix du graphisme (Bitmap brut)
            const u16* tileGfx = nullptr;

            switch (map[x][y])
            {
            case MapType::WALL: tileGfx = (u16*)tile000Bitmap; break;
            case MapType::GRASS: tileGfx = (u16*)tile037Bitmap; break;
            case MapType::WATER: tileGfx = (u16*)tile104Bitmap; break;
            case MapType::SOLID_WALL: tileGfx = (u16*)tile025Bitmap; break;
            case MapType::SAND: tileGfx = (u16*)tile061Bitmap; break;
            case MapType::FLOWER: tileGfx = (u16*)tile104Bitmap; break;
            default: continue;
            }

            // Rendu Bitmap avec test de transparence (p1 == 0x8000 ou bit 15)
            for (int l = 0; l < 32; l++)
            {
                int drawY = screenY + l;
                if (drawY < 0 || drawY >= 192)
                    continue;

                u16*       dstLine = &this->currentWritingBuffer[drawY * 256 + screenX];
                const u16* srcLine = &tileGfx[l * 32];

                for (int c = 0; c < 32; c += 2)
                {
                    int currentX = screenX + c;

                    // Clipping horizontal simple
                    if (currentX < 0 || currentX >= 255)
                        continue;

                    // Lecture 32 bits (2 pixels)
                    u32 chunk = *(u32*)&srcLine[c];
                    u16 p1    = chunk & 0xFFFF;
                    u16 p2    = chunk >> 16;

                    // Test pixel 1
                    if (!(p1 == 0x8000 || !(p1 & BIT(15))))
                    {
                        dstLine[c] = p1;
                    }

                    // Test pixel 2
                    if (!(p2 == 0x8000 || !(p2 & BIT(15))))
                    {
                        dstLine[c + 1] = p2;
                    }
                }
            }
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