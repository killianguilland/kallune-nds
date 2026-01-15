#pragma once
#include <endBottom.h>
#include <endTop.h>
#include <nds.h>
#include <okButton.h>

// #include "graphics/elements/animatedSprite.hpp"
#include "graphics/elements/background.hpp"
#include "graphics/elements/button.hpp"
#include "graphics/scene.hpp"
#include "logic/game.hpp"

class EndScene : public SceneInterface {
public:
    EndScene(const Game* /*_*/);
    ~EndScene() override;

    void draw(const Input& input) override;
    void postRender() override;

private:
    Background* backgroundTop    = {};
    Background* backgroundBottom = {};
    Button*     okButton{};
};