#pragma once
#include "utils/state.hpp"
#include "utils/router.hpp"

struct MenuState {
    ButtonState playButton {ButtonState::DEFAULT};
    ButtonState settingsButton {ButtonState::DEFAULT};

    void update(InputState state, Router* router);
};