#pragma once
#include "utils/state.hpp"

struct PlayingState {
    ButtonState pauseButton {ButtonState::DEFAULT};

    void update(InputState state);
};