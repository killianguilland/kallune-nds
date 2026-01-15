#pragma once

#include <nds.h>
#include <vector>
#include <logic/game.hpp>
#include <tile_000.h>
#include <tile_025.h>
#include <tile_037.h>
#include <tile_061.h>
#include <tile_104.h>


class Terrain
{
private:
    u16* frontBuffer;
    u16* backBuffer;
    u16* currentWritingBuffer;

public:
    Terrain();
    // posX et posY correspondent à la position de la caméra dans le monde (0 à 8192)
    void draw(const Map& mapgen, int camX, int camY);
    void swapBuffers();
    ~Terrain() = default;
};