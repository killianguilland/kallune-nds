#pragma once

#include <nds.h>
#include <cstdio>
#include "graphics/scene.hpp"
#include "input/input.hpp"
#include "logic/game.hpp"
#include "utils/router.hpp"
#include "utils/scene.hpp"
// #include "graphics/elements/oam.hpp"

class Graphics {
public:
    // Constructeur
    Graphics();

    // Destructeur
    ~Graphics() = default;

    // Update game state
    // void update(Game game, InputState inputState, Router* router);

    // Render the current frame
    void render(Router* router, Input input, const Game* game);

    void postRender(Router* router);

private:
    // SCENES
    // GameScene *game_scene {};
    // MenuScene *menu_scene {};
    // SettingsScene *settings_scene {};
    // EndScene *end_scene {};
    // PauseScene *pause_scene {};
    // LoadingScene *loading_scene {};
    SceneInterface* currentScene = nullptr;
    int             framecount{0};

    void setupScene(const Game* game, Scene scene);
};