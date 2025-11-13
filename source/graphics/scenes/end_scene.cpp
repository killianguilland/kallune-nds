#include "end_scene.hpp"

static const int DMA_CHANNEL = 3;

EndScene::EndScene(OAMTable* oam)
{
    this->oam = oam;

    int currentTileOffset = 0;

    okButton = new Button(
        oam,
        0,
        0,
        okButtonPal,
        okButtonPalLen,
        okButtonTiles,
        okButtonTilesLen,
        32,
        113,
        150,
        &currentTileOffset
    );
}

void EndScene::draw(double deltaTime, const EndState* state)
{
    okButton->draw(state->exitButton);
}