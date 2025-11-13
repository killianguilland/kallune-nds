#pragma once
#include <nds.h>
#include "utils/state.hpp"

#include <nds.h>

class Button {
public:
    Button(
        OAMTable* oam, 
        int oamId,
        int paletteId,
        const void * palette, 
        int paletteLen, 
        const void * tiles, 
        int tilesLen, 
        int spriteSize,
        int positionX,
        int positionY,
        int* currentTileOffset,
        int DMA_CHANNEL = 3
    );

    void draw(ButtonState state);

private:
    OAMTable* oam;
    int oamId;
    int paletteId;
    const void * palette;
    int paletteLen;
    const void * tiles;
    int tilesLen;
    int spriteSize;
    int positionX;
    int positionY;
    int tileOffset;
    SpriteEntry* sprite;
};
