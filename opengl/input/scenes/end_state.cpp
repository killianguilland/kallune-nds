#include "input/scenes/end_state.hpp"
#include <iostream>

void EndState::update(InputState state) {
    exitButton = checkButtonCollision(
        state, 
        0.0f, // pos X
        -3.0f,                    // pos Y
        3.8625f,                    // size X
        0.995375f                     // size Y
    );

}