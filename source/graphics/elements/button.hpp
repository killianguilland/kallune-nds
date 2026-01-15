#pragma once
#include <nds.h>
#include "utils/state.hpp"

class Button {
public:
    Button(int oamId, int paletteId, 
           const void* palette, int paletteLen, 
           const void* tiles, int tilesLen, 
           SpriteSize size, int x, int y, OamState* oam = &oamMain, u16* spritePalette = SPRITE_PALETTE);
    ~Button();

    void draw(ButtonState state);
    void hide();
    void updateGraphics(const void* tiles, int tilesLen, const void* palette, int paletteLen);

private:
    OamState* oam;
    int oamId;
    int paletteId;
    SpriteSize size;
    int posX, posY;
    void render(bool visible, ButtonState state);

    u16* gfxNormal; // Pointeur VRAM frame normale
    u16* gfxHover;  // Pointeur VRAM frame survolée
};