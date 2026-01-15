#pragma once

#include <exitButton.h>
#include <nds.h>
#include <pauseBottom.h>
#include <pauseTop.h>
#include <playButton.h>

// #include "graphics/elements/animatedSprite.hpp"
#include "graphics/elements/background.hpp"
#include "graphics/elements/button.hpp"
#include "graphics/scene.hpp"
#include "input/states/pause_state.hpp"
#include "logic/game.hpp"

class PauseScene : public SceneInterface {
public:
    PauseScene(const Game* /*_*/);
    ~PauseScene() override;

    void draw(const Input& input) override;
    void postRender() override;

private:
    Background* backgroundTop{};
    Background* backgroundBottom{};
    Button*     playButton{};
    Button*     exitButton{};
};
