#pragma once
#include "utils/state.hpp"
#include "utils/router.hpp"

struct SettingsState {
    ButtonState returnButton {DEFAULT};

    void update(InputState state, Router* router);
};