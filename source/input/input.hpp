#pragma once

// #include "input/states/menu_state.hpp"
#include <nds.h>
#include <array>
#include "input/state.hpp"
#include "input/states/end_state.hpp"
#include "input/states/menu_state.hpp"
#include "input/states/pause_state.hpp"
#include "input/states/playing_state.hpp"
#include "input/states/settings_state.hpp"
#include "utils/router.hpp"
#include "utils/state.hpp"

class Input {
private:
    // Le tableau qui contient physiquement les instances
    std::array<StateInterface*, static_cast<size_t>(Scene::COUNT)> states;

    // Un pointeur "volant" qui pointe vers l'état actif dans le tableau ci-dessus
    StateInterface* currentState = nullptr;

public:
    InputState state;
    Input();
    ~Input(); // Important pour libérer les instances à la fin

    void update(Router* router);

    // Toujours notre getter magique
    template<typename T>

    auto getState() const -> T*
    {
        // Le compilateur récupère l'index directement depuis le type T !
        size_t index = static_cast<size_t>(T::scene_type);

        return static_cast<T*>(states[index]);
    }
};