#pragma once
#include "utils/state.hpp"

struct EndState {
    ButtonState exitButton {ButtonState::DEFAULT};

    void update(InputState state);
};