#pragma once
#include <nds.h>
#include <okButton.h>
#include <settingsBottom.h>
#include <settingsTop.h>
#include <sizeButton0.h>
#include <sizeButton1.h>
#include <sizeButton2.h>
#include <sizeButton3.h>
#include <sizeButton4.h>
#include <typeButton0.h>
#include <typeButton1.h>
#include <waterButton0.h>
#include <waterButton1.h>
#include <waterButton2.h>
#include <waterButton3.h>
#include <waterButton4.h>
#include <cstdio>

// #include "graphics/elements/animatedSprite.hpp"
#include "graphics/elements/background.hpp"
#include "graphics/elements/button.hpp"
#include "graphics/scene.hpp"
#include "input/scenes/settings_state.hpp"
#include "logic/game.hpp"
#include "utils/state.hpp"

class SettingsScene : public SceneInterface {
public:
    SettingsScene(const Game* /*_*/);
    ~SettingsScene() override;

    void draw(const Input& input) override;
    void postRender() override;

private:
    Background* backgroundTop{};
    Background* backgroundBottom{};

    Button* returnButton{};

    Button* waterBtn;
    Button* sizeBtn;
    Button* typeBtn;

    int lastWater = -1;
    int lastSize  = -1;
    int lastType  = -1;
};