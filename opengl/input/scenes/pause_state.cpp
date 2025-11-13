#include "input/scenes/pause_state.hpp"
#include <iostream>

void PauseState::update(InputState state) {
    returnButton = checkButtonCollision(
        state, 
        -2.0f, // pos X
        -0.25f,                    // pos Y
        3.31f,                    // size X
        0.99f                     // size Y
    );
    exitButton = checkButtonCollision(
        state, 
        2.0f, 
        -0.25f,
        3.31f,
        0.99f
    );
}