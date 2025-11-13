#include "input.hpp"
#include <iostream>
#include <stdio.h>

void Input::update(Game* game, Router* router) {
    // Update the key registers with current values.
    scanKeys();

    // Update the touch screen values.
    touchRead(&state.touch);

    int keys = keysHeld();

    state.touchUp = false;
    state.touching = (keys & KEY_TOUCH);

    // If the stylus is touching, update lastTouch
    if (state.touching) {
        state.lastTouch = state.touch;
    }

    // Detect when the stylus was touching but now isn't
    if (state.wasTouching && !state.touching) {
        state.touchUp = true;
        fprintf(stderr, "Touch up at: X=%d, Y=%d\n", state.lastTouch.px, state.lastTouch.py);
    }

    state.wasTouching = state.touching;

    switch (router->currentScene)
    {
    case Scene::Menu:
        menu_state->update(state, router);
        break;

    case Scene::Settings:
        settings_state->update(state, router);
        break;

    case Scene::Playing:
        if (!game->isPlayerAlive())
        {
            router->goTo(Scene::End);
            return;
        }
        else
        {
            playing_state->update(state, router);
            break;
        }
        break;

    case Scene::Pause:
        pause_state->update(state, router);
        break;
    
    case Scene::End:
        end_state->update(state, router);
        break;

    default:
        std::cerr << "Euh y a un truc pas normal chef" << std::endl;
        break;
    }
}

Input::Input() {
    // Setup callbacks
    menu_state = new MenuState();
    settings_state = new SettingsState();
    pause_state = new PauseState();
    end_state = new EndState();
    playing_state = new PlayingState();
}