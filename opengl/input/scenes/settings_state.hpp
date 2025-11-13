#pragma once
#include "utils/state.hpp"

struct SettingsState {
    ButtonState returnButton {DEFAULT};

    void update(InputState state);
};