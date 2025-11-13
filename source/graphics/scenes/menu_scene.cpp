#include "menu_scene.hpp"
#include <graphics/elements/oam.hpp>

static const int DMA_CHANNEL = 3;

MenuScene::MenuScene(OAMTable* oam) {
    this->oam = oam;

    int currentTileOffset = 0;

    startButton = new Button(
        oam,
        0,  // oamId
        0,  // paletteId
        startButtonPal,
        startButtonPalLen,
        startButtonTiles,
        startButtonTilesLen,
        64, // sprite size
        77, // X
        150, // Y
        &currentTileOffset
    );

    settingsButton = new Button(
        oam,
        1,
        1,
        settingsButtonPal,
        settingsButtonPalLen,
        settingsButtonTiles,
        settingsButtonTilesLen,
        32,
        148,
        150,
        &currentTileOffset
    );

    // SPRITES

    // // Load start button
    // dmaCopyHalfWords(
    //     DMA_CHANNEL, 
    //     startButtonPal, 
    //     SPRITE_PALETTE, 
    //     startButtonPalLen
    // );
    // dmaCopyHalfWords(
    //     DMA_CHANNEL, 
    //     startButtonTiles, 
    //     SPRITE_GFX, 
    //     startButtonTilesLen
    // );
    // startButtonSprite = &oam->oamBuffer[0];
    // startButtonSprite->attribute[0] = ATTR0_COLOR_16 | ATTR0_SQUARE | 150; // Position Y
    // startButtonSprite->attribute[1] = ATTR1_SIZE_64 | 77;                  // Position X
    // startButtonSprite->attribute[2] = 0 | ATTR2_PRIORITY(0) | ATTR2_PALETTE(0);

    // fprintf(stderr, "FC Attr0: %d\n", startButtonSprite->attribute[0]);
    // fprintf(stderr, "FC Attr1: %d\n", startButtonSprite->attribute[1]);
    // fprintf(stderr, "FC Attr2: %d\n", startButtonSprite->attribute[2]);

    // // Load settings button
    // dmaCopyHalfWords(DMA_CHANNEL, 
    //     settingsButtonPal, 
    //     &SPRITE_PALETTE[16], 
    //     settingsButtonPalLen
    // );
    // dmaCopyHalfWords(
    //     DMA_CHANNEL,
    //     settingsButtonTiles,
    //     SPRITE_GFX + (startButtonTilesLen / 2),
    //     settingsButtonTilesLen
    // );
    // settingsButtonSprite = &oam->oamBuffer[1];
    // settingsButtonSprite->attribute[0] = ATTR0_COLOR_16 | ATTR0_SQUARE | 150; // Position Y
    // settingsButtonSprite->attribute[1] = ATTR1_SIZE_32 | 148;                 // Position X
    // settingsButtonSprite->attribute[2] = (startButtonTilesLen / 32) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(1);

}

void MenuScene::draw(double deltaTime, const MenuState* menuState)
{
    startButton->draw(menuState->playButton);
    // if(menuState->playButton == ButtonState::HOVER) {
    //     startButtonSprite->attribute[2] = 64     // tile index = 64 (pressed state)
    //     | ATTR2_PRIORITY(0)
    //     | ATTR2_PALETTE(0);  // use palette 0
    // } else {
    //     startButtonSprite->attribute[2] = 0      // tile index = 0
    //     | ATTR2_PRIORITY(0)
    //     | ATTR2_PALETTE(0);  // use palette 0
    // }
    
    settingsButton->draw(menuState->settingsButton);
    // if(menuState->settingsButton == ButtonState::HOVER) {
    //     settingsButtonSprite->attribute[2] = (startButtonTilesLen / 32) + 16 // tile index = 0
    //                         | ATTR2_PRIORITY(0)
    //                         | ATTR2_PALETTE(1);  // use palette 1
    // } else {
    //     settingsButtonSprite->attribute[2] = (startButtonTilesLen / 32) // tile index = 4 (pressed state)
    //                         | ATTR2_PRIORITY(0)
    //                         | ATTR2_PALETTE(1);  // use palette 1
    // }
}