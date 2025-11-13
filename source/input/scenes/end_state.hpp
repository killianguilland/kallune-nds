#pragma once
#include "utils/state.hpp"
#include "utils/router.hpp"

struct EndState {
    ButtonState exitButton {ButtonState::DEFAULT};

    void update(InputState state, Router* router);
};