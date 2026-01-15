#include "loading_scene.hpp"
#include <graphics/elements/oam.hpp>
#include "graphics/hardware.hpp"

LoadingScene::LoadingScene(const Game* /*_*/)
{
    Hardware::setupUILayout();

    backgroundTop    = new Background(loadingTopBitmap, loadingTopBitmapLen, true);
    backgroundBottom = new Background(loadingBottomBitmap, loadingBottomBitmapLen, false);
}

void LoadingScene::draw(const Input& input)
{
}
void LoadingScene::postRender()
{
}

LoadingScene::~LoadingScene()
{
}