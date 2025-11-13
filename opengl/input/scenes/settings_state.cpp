#include "settings_state.hpp"

void SettingsState::update(InputState state) {
    returnButton = checkButtonCollision(
        state, 
        -8.0f + 1.55f / 2 + 1.6f, 
        -3.0f,
        1.55f,
        0.99f
    );
}