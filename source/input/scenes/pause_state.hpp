#pragma once
#include "utils/state.hpp"
#include "utils/router.hpp"

struct PauseState {
    ButtonState returnButton {ButtonState::DEFAULT};
    ButtonState exitButton {ButtonState::DEFAULT};

    void update(InputState state, Router* router);
};