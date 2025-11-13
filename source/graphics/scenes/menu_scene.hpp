#pragma once
#include <nds.h>
#include <stdio.h>
#include <menuTop.h>
#include <menuBottom.h>
#include <startButton.h>
#include <settingsButton.h>

// #include "graphics/elements/animatedSprite.hpp"
#include "input/scenes/menu_state.hpp"
#include "utils/scene.hpp"
#include "utils/router.hpp"
#include "graphics/elements/background.hpp"
#include "graphics/elements/button.hpp"

class MenuScene
{
public:
    MenuScene(OAMTable* oam);
    ~MenuScene() = default;

    void draw(double deltaTime, const MenuState* state);

private:
    OAMTable* oam {nullptr};
    Background* backgroundTop = new Background(menuTopBitmap, menuTopBitmapLen, true);
    Background* backgroundBottom = new Background(menuBottomBitmap, menuBottomBitmapLen, false);
    Button* startButton {};
    Button* settingsButton {};
    SpriteEntry* settingsButtonSprite;
    SpriteEntry* startButtonSprite;
};