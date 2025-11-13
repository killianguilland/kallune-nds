#pragma once

#include "GLFW/glfw3.h"
#include "utils/scene.hpp"
#include "logic/game.hpp"
#include "graphics/scenes/game_scene.hpp"
#include "graphics/scenes/menu_scene.hpp"
#include "graphics/scenes/settings_scene.hpp"
#include "graphics/scenes/end_scene.hpp"
#include "utils/state.hpp"
#include "utils/router.hpp"
#include "graphics/scenes/pause_scene.hpp"

class Graphics {
public:
    // Constructeur
    Graphics();

    // Destructeur
    ~Graphics() = default;

    // Update game state 
    void update(double deltaTime, Game game, InputState inputState, Router* router);

    // Render the current frame
    void render(double deltaTime, Router* router, InputState inputState, Game game);

    // Checks if the user requested to close the window
    bool shouldClose();

    void close();

    GLFWwindow *window {};

private:
    static void onError(int error, const char *description);

    static void onWindowResized(GLFWwindow *, int width, int height);

    // SCENES
    GameScene *game_scene {};
    MenuScene *menu_scene {};
    SettingsScene *settings_scene {};
    EndScene *end_scene {};
    PauseScene *pause_scene {};

    // CURSOR
    const double CURSOR_ANIMATION_DURATION = 0.5;
    Sprite *cursorSprite{};
    AnimatedSprite *cursorAnimatedSprite{};
};