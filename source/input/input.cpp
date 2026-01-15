#include "input.hpp"
#include <array>
#include <cstdio>
#include "input/state.hpp"
#include "input/states/end_state.hpp"
#include "input/states/loading_state.hpp"
#include "input/states/menu_state.hpp"
#include "input/states/pause_state.hpp"
#include "input/states/playing_state.hpp"
#include "input/states/settings_state.hpp"

template<typename T>
static auto stateCreator() -> StateInterface*
{
    return new T();
}

using StateFactory = StateInterface* (*)();

static const std::array<StateFactory, Scene::COUNT> stateRegistry = {
    stateCreator<MenuState>,
    stateCreator<SettingsState>,
    stateCreator<LoadingState>,
    stateCreator<PlayingState>,
    stateCreator<PauseState>,
    stateCreator<EndState>
};

Input::Input()
{
    // 1. On initialise tous les états au démarrage
    for (size_t i = 0; i < states.size(); ++i)
    {
        // On utilise la factory pour créer chaque objet
        states[i] = stateRegistry[i]();
    }

    // Par défaut, on pointe sur le menu (ou l'état de départ)
    currentState = states[static_cast<size_t>(Scene::Menu)];
}

Input::~Input()
{
    // On libère proprement la mémoire au moment où l'objet Input est détruit
    // for (auto s : states)
    // {
    //     if (s)
    //         delete s;
    // }
}

void Input::update(Router* router)
{
    // Update the key registers with current values.
    scanKeys();

    // Update the touch screen values.
    touchRead(&state.touch);

    state.keys = keysHeld();

    state.touchUp  = false;
    state.touching = (state.keys & KEY_TOUCH);

    state.moveX = state.keys & KEY_RIGHT ? 1 : (state.keys & KEY_LEFT ? -1 : 0);
    state.moveY = state.keys & KEY_DOWN ? 1 : (state.keys & KEY_UP ? -1 : 0);

    // If the stylus is touching, update lastTouch
    if (state.touching)
    {
        state.lastTouch = state.touch;
    }

    // Detect when the stylus was touching but now isn't
    if (state.wasTouching && !state.touching)
    {
        state.touchUp = true;
        fprintf(stderr, "Touch up at: X=%d, Y=%d\n", state.lastTouch.px, state.lastTouch.py);
    }

    state.wasTouching = state.touching;

    // Changement d'état : on change juste l'adresse vers laquelle on pointe
    states[static_cast<size_t>(router->currentScene)]->update(state, router);
}