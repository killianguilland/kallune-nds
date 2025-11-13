#pragma once
#include <nds.h>
#include <stdio.h>
#include <settingsTop.h>
#include <settingsBottom.h>
#include <okButton.h>

// #include "graphics/elements/animatedSprite.hpp"
#include "input/scenes/settings_state.hpp"
#include "utils/scene.hpp"
#include "utils/router.hpp"
#include "graphics/elements/background.hpp"
#include "graphics/elements/button.hpp"

class SettingsScene
{
public:
    SettingsScene(OAMTable* oam);
    ~SettingsScene() = default;

    void draw(double deltaTime, const SettingsState* state);

private:
    OAMTable* oam {nullptr};
    Background* backgroundTop = new Background(settingsTopBitmap, settingsTopBitmapLen, true);
    Background* backgroundBottom = new Background(settingsBottomBitmap, settingsBottomBitmapLen, false);
    Button* returnButton {};
};