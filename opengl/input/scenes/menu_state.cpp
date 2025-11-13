#include "menu_state.hpp"

void MenuState::update(InputState state) {
    playButton = checkButtonCollision(
        state, 
        -8.0f + 3.31f / 2 + 0.8f, // pos X
        -0.5f,                    // pos Y
        3.31f,                    // size X
        0.99f                     // size Y
    );
    settingsButton = checkButtonCollision(
        state, 
        -8.0f + 1.55f / 2 + 0.8f, 
        -1.75f, 
        1.55f, 
        0.99f
    );
    exitButton = checkButtonCollision(
        state, 
        -8.0f + 1.55f + 1.8f, 
        -1.75f,
        1.55f,
        0.99f
    );
}
