#include "menu_scene.hpp"
#include <graphics/elements/oam.hpp>
#include "graphics/hardware.hpp"
#include "input/scenes/menu_state.hpp"

MenuScene::MenuScene(const Game* /*_*/)
{
    Hardware::setupUILayout();

    backgroundTop    = new Background(menuTopBitmap, menuTopBitmapLen, true);
    backgroundBottom = new Background(menuBottomBitmap, menuBottomBitmapLen, false);

    startButton = new Button(
        0, // oamId
        0, // paletteId
        startButtonPal,
        startButtonPalLen,
        startButtonTiles,
        startButtonTilesLen,
        SpriteSize_64x64, // sprite size
        77,               // X
        150               // Y
    );

    settingsButton = new Button(
        1,
        1,
        settingsButtonPal,
        settingsButtonPalLen,
        settingsButtonTiles,
        settingsButtonTilesLen,
        SpriteSize_32x32,
        148,
        150
    );
}

void MenuScene::draw(const Input& input)
{
    const MenuState* menuState = input.getMenuState();
    startButton->draw(menuState->playButton);
    settingsButton->draw(menuState->settingsButton);
}

void MenuScene::postRender()
{
}

MenuScene::~MenuScene()
{
    delete startButton;
    delete settingsButton;
    oamClear(&oamMain, 0, 0);
}