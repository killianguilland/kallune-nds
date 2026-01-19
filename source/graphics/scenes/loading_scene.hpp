#pragma once
#include <loadingBottom.h>
#include <loadingTop.h>
#include <nds.h>
#include <cstdio>

// #include "graphics/elements/animatedSprite.hpp"
#include "graphics/elements/background.hpp"
#include "graphics/scene.hpp"
#include "logic/game.hpp"

class LoadingScene : public SceneInterface {
public:
    LoadingScene(const Game* /*_*/);
    ~LoadingScene() override;

    void draw(const Input& input) override;
    void postRender() override;

private:
    Background* backgroundTop{};
    Background* backgroundBottom{};
};