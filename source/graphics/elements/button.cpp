#include "button.hpp"

Button::Button(int oamId, int paletteId, const void* palette, int paletteLen, const void* tiles, int tilesLen, SpriteSize size, int x, int y, OamState* oam, u16* spritePalette)
    : oamId(oamId), paletteId(paletteId), size(size), posX(x), posY(y), oam(oam)
{
    // 1. Charger la palette dans la banque correspondante
    dmaCopyHalfWords(3, palette, &spritePalette[paletteId * 16], paletteLen);

    // 2. Allouer de la mémoire VRAM pour la frame NORMALE
    gfxNormal = oamAllocateGfx(oam, size, SpriteColorFormat_16Color);

    // 3. Allouer de la mémoire VRAM pour la frame HOVER
    gfxHover = oamAllocateGfx(oam, size, SpriteColorFormat_16Color);

    // 4. Copier les données. On divise tilesLen par 2 car on a 2 frames dans le fichier.
    int bytesPerFrame = tilesLen / 2;

    // Copie de la première moitié du fichier (Normal)
    dmaCopyHalfWords(3, tiles, gfxNormal, bytesPerFrame);

    // Copie de la deuxième moitié du fichier (Hover)
    dmaCopyHalfWords(3, (u8*)tiles + bytesPerFrame, gfxHover, bytesPerFrame);
}

void Button::draw(ButtonState state)
{
    this->render(false, state);
}

void Button::hide()
{
    this->render(true, ButtonState::DEFAULT);
}

void Button::render(bool isHidden, ButtonState state)
{
    // On choisit le pointeur VRAM en fonction de l'état
    u16* currentGfx = (state == ButtonState::HOVER) ? gfxHover : gfxNormal;

    oamSet(oam, oamId, posX, posY,
           0,         // Priorité (0-3)
           paletteId, // Index de la palette (0-15)
           size, SpriteColorFormat_16Color,
           currentGfx, // Le pointeur magique alloué
           -1, false, isHidden, false, false, false);
}

void Button::updateGraphics(const void* tiles, int tilesLen, const void* palette, int paletteLen)
{
    // 1. Mise à jour des tuiles (VRAM)
    int bytesPerFrame = tilesLen / 2;
    dmaCopyHalfWords(3, tiles, gfxNormal, bytesPerFrame);
    dmaCopyHalfWords(3, (u8*)tiles + bytesPerFrame, gfxHover, bytesPerFrame);

    // 2. Mise à jour de la palette (RAM Palette)
    // On calcule l'adresse de départ : paletteId * 16 couleurs
    dmaCopyHalfWords(3, palette, &SPRITE_PALETTE[this->paletteId * 16], paletteLen);
}

Button::~Button()
{
    // Libère la mémoire VRAM pour que d'autres puissent l'utiliser
    if (gfxNormal)
        oamFreeGfx(oam, gfxNormal);
    if (gfxHover)
        oamFreeGfx(oam, gfxHover);
}