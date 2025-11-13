#include "game_scene.hpp"

GameScene::GameScene(OAMTable* oam)
{
    this->oam = oam;
    int currentTileOffset = 0;

    // Each pixel = 16 bits (u16), laid out row by row.
    // Mode 5 = 160×128 pixels, Mode 3 = 256×256 pixels
    int x = 10;
    int y = 10;

    // Make sure you're in the correct range for your mode!
    int width = 256; // Mode 5 width
    u16 color = RGB15(31, 0, 0) | BIT(15);  // Red with alpha bit set


    pauseButtonTiles[y * width + x] = color;

    pauseButton = new Button(
        oam,
        0,
        0,
        pauseButtonPal,
        pauseButtonPalLen,
        pauseButtonTiles,
        pauseButtonTilesLen,
        32,
        113,
        150,
        &currentTileOffset
    );

    u16* bgPixels = (u16*)BG_BMP_RAM_SUB(0);

    bgPixels[y * width + x] = color;
}

void GameScene::update(Router* router) {
}

void GameScene::draw(double deltaTime, const PlayingState* playingState)
{
    pauseButton->draw(playingState->pauseButton);
}