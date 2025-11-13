#pragma once
#include "utils/state.hpp"
#include "utils/router.hpp"

struct PlayingState {
    ButtonState pauseButton {ButtonState::DEFAULT};

    void update(InputState state, Router* router);
};