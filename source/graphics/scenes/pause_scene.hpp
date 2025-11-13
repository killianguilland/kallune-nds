#pragma once

#include <pauseBottom.h>
#include <pauseTop.h>
#include <nds.h>
#include <playButton.h>

// #include "graphics/elements/animatedSprite.hpp"
#include "input/scenes/pause_state.hpp"
#include "utils/scene.hpp"
#include "utils/router.hpp"
#include "graphics/elements/background.hpp"
#include "graphics/elements/button.hpp"

class PauseScene
{
public:
    PauseScene(OAMTable* oam);
    ~PauseScene() = default;

    void draw(double deltaTime, const PauseState* state);

private:
    OAMTable* oam {nullptr};
    Background* backgroundTop = new Background(pauseTopBitmap, pauseTopBitmapLen, true);
    Background* backgroundBottom = new Background(pauseBottomBitmap, pauseBottomBitmapLen, false);
    Button* playButton {};
};