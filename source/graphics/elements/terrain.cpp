#include "terrain.hpp"

// On suppose que tes fichiers grit sont inclus ici
// #include "all_tiles.h"

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
    // 1. Effacement rapide avec DMA
    dmaFillWords(RGB15(0, 0, 0) | BIT(15), this->currentWritingBuffer, 256 * 192 * 2);

    const std::vector<std::vector<MapType>>& map     = mapgen.getMap();
    int                                      mapSize = mapgen.getWidth();

    int isoScrollX = (camX - camY) * 16;
    int isoScrollY = (camX + camY) * 8;

    // 2. Culling amélioré : On calcule les bornes une seule fois
    int range       = 10; // Nombre de cases affichées autour du centre
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
            // On calcule la position écran relative au centre isométrique
            int screenX = (x - y) * 16 - isoScrollX + 128;
            int screenY = (x + y) * 8 - isoScrollY + 96 - 16; // 96 pour centrer en Y

            // Culling tuile entière
            if (screenX < -32 || screenX >= 256 || screenY < -32 || screenY >= 192)
                continue;

            // 4. Choix du graphisme (Remplace par ton tableau terrainTilesTable pour plus de vitesse)

            const u16* tileGfx = nullptr;

            switch (map[x][y])
            {
            case MapType::WALL:
                tileGfx = reinterpret_cast<const u16*>(tile000Bitmap);
                break;

            case MapType::GRASS:
                tileGfx = reinterpret_cast<const u16*>(tile037Bitmap);
                break;

            case MapType::WATER:
                tileGfx = reinterpret_cast<const u16*>(tile104Bitmap);
                break;

            case MapType::SOLID_WALL:
                tileGfx = reinterpret_cast<const u16*>(tile025Bitmap);
                break;

            case MapType::SAND:
                tileGfx = reinterpret_cast<const u16*>(tile061Bitmap);
                break;

            case MapType::FLOWER:
                tileGfx = reinterpret_cast<const u16*>(tile104Bitmap);
                break;
            }

            // 5. Dessin optimisé 32 bits
            for (int l = 0; l < 32; l++)
            {
                int drawY = screenY + l;
                if (drawY < 0 || drawY >= 192)
                    continue;

                // On pré-calcule l'adresse de la ligne dans le buffer de destination
                u16*       dstLine = &this->currentWritingBuffer[drawY * 256 + screenX];
                const u16* srcLine = &tileGfx[l * 32];

                // Boucle de colonnes par pas de 2 pixels (32 bits)
                for (int c = 0; c < 32; c += 2)
                {
                    int currentX = screenX + c;

                    // Clipping horizontal rapide
                    if (currentX < 0 || currentX >= 255)
                        continue;

                    // --- MAGIE DU 32 BITS ---
                    // On lit 2 pixels d'un coup (u32)
                    u32 chunk = *(u32*)&srcLine[c];

                    // Extraction des deux pixels
                    u16 p1 = chunk & 0xFFFF;
                    u16 p2 = chunk >> 16;

                    // Test de transparence pour le pixel 1
                    if (!(p1 == 0x8000 || !(p1 & BIT(15))))
                    {
                        dstLine[c] = p1;
                    }

                    // Test de transparence pour le pixel 2
                    if (!(p2 == 0x8000 || !(p2 & BIT(15))))
                    {
                        dstLine[c + 1] = p2;
                    }
                }
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