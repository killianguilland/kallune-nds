#pragma once

#include <nds.h>
#include <gameBottom.h>
#include <gameTop.h>
#include <pauseButton.h>

#include "utils/scene.hpp"
#include "utils/router.hpp"
#include "graphics/elements/background.hpp"
#include "graphics/elements/button.hpp"
#include "input/scenes/playing_state.hpp"

class GameScene
{
public:
    GameScene(OAMTable* oam);
    ~GameScene() = default;

    void update(Router* router);
    void draw(double deltaTime, const PlayingState* playingState);

private:
    OAMTable* oam {nullptr};
    Background* backgroundTop = new Background(gameTopBitmap, gameTopBitmapLen, true);
    Background* backgroundBottom = new Background(gameBottomBitmap, gameBottomBitmapLen, false);
    Button* pauseButton {};
};