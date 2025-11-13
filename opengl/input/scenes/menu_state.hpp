#pragma once
#include "utils/state.hpp"

struct MenuState {
    ButtonState playButton {ButtonState::DEFAULT};
    ButtonState settingsButton {ButtonState::DEFAULT};
    ButtonState exitButton {ButtonState::DEFAULT};

    void update(InputState state);
};