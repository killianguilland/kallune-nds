#pragma once
#include <nds.h>
#include <endBottom.h>
#include <endTop.h>
#include <okButton.h>

// #include "graphics/elements/animatedSprite.hpp"
#include "input/scenes/end_state.hpp"
#include "utils/scene.hpp"
#include "utils/router.hpp"
#include "graphics/elements/background.hpp"
#include "graphics/elements/button.hpp"

class EndScene
{
public:
    EndScene(OAMTable* oam);
    ~EndScene() = default;

    void draw(double deltaTime, const EndState* state);

private:
    OAMTable* oam {nullptr};
    Background* backgroundTop = new Background(endTopBitmap, endTopBitmapLen, true);
    Background* backgroundBottom = new Background(endBottomBitmap, endBottomBitmapLen, false);
    Button* okButton {};
};