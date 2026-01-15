#pragma once

#include <gameBottom.h>
#include <gameTop.h>
#include <nds.h>
#include <pauseButton.h>
#include "graphics/elements/background.hpp"
#include "graphics/elements/button.hpp"
#include "graphics/elements/minimap.hpp"
#include "graphics/elements/terrain.hpp"
#include "graphics/scene.hpp"
#include "input/states/playing_state.hpp"
#include "logic/game.hpp"

class GameScene : public SceneInterface {
public:
    GameScene(const Game* game);
    ~GameScene() override;

    void draw(const Input& input) override;
    void postRender() override;

private:
    // Background* backgroundTop = new Background(gameTopBitmap, gameTopBitmapLen, false);
    Background* backgroundBottom{};
    Button*     pauseButton{};
    const Game* game{nullptr};
    Minimap     minimap;
    Terrain     terrain{};
    bool        badgerMoved{true};
    float       x{0};
    float       y{0};
};