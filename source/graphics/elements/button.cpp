#include "button.hpp"
#include <stdexcept>
#include <stdio.h>

// Offset global pour la VRAM des tiles
int currentTileOffset = 0;

Button::Button(
    OAMTable* oam, 
    const int oamId,
    const int paletteId,
    const void * palette, 
    const int paletteLen, 
    const void * tiles, 
    const int tilesLen, 
    const int spriteSize,
    const int positionX,
    const int positionY,
    int* currentTileOffset,
    const int DMA_CHANNEL
) 
    : oam{oam}, 
      oamId{oamId}, 
      paletteId{paletteId},
      palette{palette}, 
      paletteLen{paletteLen}, 
      tiles{tiles}, 
      tilesLen{tilesLen}, 
      spriteSize{spriteSize},
      positionX{positionX}, 
      positionY{positionY}
{
    tileOffset = *currentTileOffset;

    // --- Copier la palette ---
    dmaCopyHalfWords(
        DMA_CHANNEL, 
        palette, 
        &SPRITE_PALETTE[paletteId * 16], 
        paletteLen
    );

    // --- Copier les tiles à l'offset courant ---
    dmaCopyHalfWords(
        DMA_CHANNEL, 
        tiles, 
        SPRITE_GFX + tileOffset, 
        tilesLen
    );

    // Calcul de l’index de tile
    int tileIndex = tileOffset / 16;

    fprintf(stderr, "Button %d: tileOffset=%d, tileIndex=%d\n", oamId, tileOffset, tileIndex);

    // --- Configurer le sprite ---
    sprite = &oam->oamBuffer[oamId];
    sprite->attribute[0] = ATTR0_COLOR_16 | ATTR0_SQUARE | positionY;

    switch (spriteSize) {
        case 8:  sprite->attribute[1] = ATTR1_SIZE_8  | positionX; break;
        case 16: sprite->attribute[1] = ATTR1_SIZE_16 | positionX; break;
        case 32: sprite->attribute[1] = ATTR1_SIZE_32 | positionX; break;
        case 64: sprite->attribute[1] = ATTR1_SIZE_64 | positionX; break;
        default:
            fprintf(stderr, "Invalid sprite size: %d\n", spriteSize);
    }

    sprite->attribute[2] = tileIndex | ATTR2_PRIORITY(0) | ATTR2_PALETTE(paletteId);

    // Debug
    fprintf(stderr, "Button %d loaded (tileIndex=%d, offset=%d)\n", oamId, tileIndex, *currentTileOffset);

    // --- Incrémenter l'offset global pour le prochain sprite ---
    *currentTileOffset += tilesLen / 2; // halfwords
}

void Button::draw(ButtonState state) {
    if(state == ButtonState::HOVER) {
        int tilesAcross = spriteSize / 8;
        int tilesPerSprite = tilesAcross * tilesAcross; // (spriteSize/8)^2
        sprite->attribute[2] = (tileOffset / 16 + tilesPerSprite)
                            | ATTR2_PRIORITY(0)
                            | ATTR2_PALETTE(paletteId);  // use palette 0
    } else {
        sprite->attribute[2] = (tileOffset / 16)
                            | ATTR2_PRIORITY(0)
                            | ATTR2_PALETTE(paletteId);  // use palette 0
    }
}