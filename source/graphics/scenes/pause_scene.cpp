#include "pause_scene.hpp"
#include "graphics/hardware.hpp"
#include "input/states/pause_state.hpp"

PauseScene::PauseScene(const Game* /*_*/)
{
    Hardware::setupUILayout();

    backgroundTop    = new Background(pauseTopBitmap, pauseTopBitmapLen, true);
    backgroundBottom = new Background(pauseBottomBitmap, pauseBottomBitmapLen, false);

    exitButton = new Button(
        0, // oamId
        0, // paletteId
        exitButtonPal,
        exitButtonPalLen,
        exitButtonTiles,
        exitButtonTilesLen,
        SpriteSize_64x64, // sprite size
        77,               // X
        150               // Y
    );

    playButton = new Button(
        1,
        1,
        playButtonPal,
        playButtonPalLen,
        playButtonTiles,
        playButtonTilesLen,
        SpriteSize_32x32,
        148,
        150
    );
}

void PauseScene::draw(const Input& input)
{
    const PauseState* pauseState = input.getState<PauseState>();
    exitButton->draw(pauseState->exitButton);
    playButton->draw(pauseState->returnButton);
}

void PauseScene::postRender()
{
}

PauseScene::~PauseScene()
{
    delete exitButton;
    delete playButton;
    oamClear(&oamMain, 0, 0);
}