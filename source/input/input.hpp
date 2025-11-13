#pragma once
#include <array>

#include "utils/scene.hpp"
// #include "input/scenes/menu_state.hpp"
#include "utils/state.hpp"
#include "utils/router.hpp"
#include <nds.h>
#include "input/scenes/menu_state.hpp"
#include "input/scenes/playing_state.hpp"
#include "input/scenes/settings_state.hpp"
#include "input/scenes/pause_state.hpp"
#include "input/scenes/end_state.hpp"
#include "logic/game.hpp"

class Input {

private:
    MenuState* menu_state;
    SettingsState* settings_state;
    PauseState* pause_state;
    PlayingState* playing_state;
    EndState* end_state;

public:
    Input();
    void update(Game* game, Router* router);

    // Read-only getters for states
    const MenuState* getMenuState() const { return menu_state; }
    const PlayingState* getPlayingState() const { return playing_state; }
    const SettingsState* getSettingsState() const { return settings_state; }
    const PauseState* getPauseState() const { return pause_state; }
    const EndState* getEndState() const { return end_state; }

    InputState state {InputState()};


};