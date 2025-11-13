#include "input/scenes/playing_state.hpp"
#include <iostream>

void PlayingState::update(InputState state) {
    pauseButton = checkButtonCollision(
        state, 
        7.0f,                   // pos X
        3.5f,                    // pos Y
        1.0625f,                    // size X
        0.925f                     // size Y
    );

}