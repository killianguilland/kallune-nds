#include "pause_scene.hpp"

PauseScene::PauseScene(OAMTable* oam)
{
    this->oam = oam;

    int currentTileOffset = 0;

    playButton = new Button(
        oam,
        0,
        0,
        playButtonPal,
        playButtonPalLen,
        playButtonTiles,
        playButtonTilesLen,
        32,
        113,
        150,
        &currentTileOffset
    );
}

void PauseScene::draw(double deltaTime, const PauseState* pauseState)
{
    playButton->draw(pauseState->returnButton);
}