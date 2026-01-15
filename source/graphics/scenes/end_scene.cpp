#include "end_scene.hpp"
#include "graphics/hardware.hpp"
#include "input/scenes/end_state.hpp"

static const int DMA_CHANNEL = 3;

EndScene::EndScene(const Game* /*game*/)
{
    Hardware::setupUILayout();

    backgroundTop    = new Background(endTopBitmap, endTopBitmapLen, true);
    backgroundBottom = new Background(endBottomBitmap, endBottomBitmapLen, false);

    okButton = new Button(
        0,
        0,
        okButtonPal,
        okButtonPalLen,
        okButtonTiles,
        okButtonTilesLen,
        SpriteSize_32x32,
        113,
        150
    );
}

void EndScene::draw(const Input& input)
{
    const EndState* endState = input.getEndState();
    okButton->draw(endState->exitButton);
}

void EndScene::postRender()
{
}

EndScene::~EndScene()
{
    delete okButton;
    oamClear(&oamMain, 0, 0);
}