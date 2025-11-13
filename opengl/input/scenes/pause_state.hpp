#pragma once
#include "utils/state.hpp"

struct PauseState {
    ButtonState returnButton {ButtonState::DEFAULT};
    ButtonState exitButton {ButtonState::DEFAULT};

    void update(InputState state);
};