#pragma once

#include "utils/scene.hpp"
#include "logic/game.hpp"
#include "graphics/scenes/game_scene.hpp"
#include "graphics/scenes/menu_scene.hpp"
#include "graphics/scenes/settings_scene.hpp"
#include "graphics/scenes/end_scene.hpp"
#include "graphics/scenes/pause_scene.hpp"
#include "utils/state.hpp"
#include "input/input.hpp"
#include "utils/router.hpp"
#include "graphics/elements/oam.hpp"

class Graphics {
public:
    // Constructeur
    Graphics();

    // Destructeur
    ~Graphics() = default;

    // Update game state 
    // void update(double deltaTime, Game game, InputState inputState, Router* router);

    // Render the current frame
    void render(double deltaTime, Router* router, Input input, Game game);

    void postRender();

private:
	OAMTable *oam = new OAMTable();

    // SCENES
    GameScene *game_scene {};
    MenuScene *menu_scene {};
    SettingsScene *settings_scene {};
    EndScene *end_scene {};
    PauseScene *pause_scene {};

    void setupScene(Scene scene);
};