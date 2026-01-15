#pragma once
#include <menuBottom.h>
#include <menuTop.h>
#include <nds.h>
#include <settingsButton.h>
#include <startButton.h>
#include <stdio.h>

// #include "graphics/elements/animatedSprite.hpp"
#include "graphics/elements/background.hpp"
#include "graphics/elements/button.hpp"
#include "graphics/scene.hpp"
#include "logic/game.hpp"

class MenuScene : public SceneInterface {
public:
    MenuScene(const Game* /*_*/);
    ~MenuScene() override;

    void draw(const Input& input) override;
    void postRender() override;

private:
    Background*  backgroundTop{};
    Background*  backgroundBottom{};
    Button*      startButton{};
    Button*      settingsButton{};
    SpriteEntry* settingsButtonSprite;
    SpriteEntry* startButtonSprite;
};