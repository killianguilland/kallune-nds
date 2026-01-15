#include "loading_scene.hpp"
#include <graphics/elements/oam.hpp>
#include "graphics/hardware.hpp"
#include "input/states/loading_state.hpp"

LoadingScene::LoadingScene(const Game* /*_*/)
{
    Hardware::setupUILayout();

    backgroundTop    = new Background(loadingTopBitmap, loadingTopBitmapLen, true);
    backgroundBottom = new Background(loadingBottomBitmap, loadingBottomBitmapLen, false);
}

void LoadingScene::draw(const Input& input)
{
    const LoadingState* loadingState = input.getState<LoadingState>();
}
void LoadingScene::postRender()
{
}

LoadingScene::~LoadingScene()
{
}