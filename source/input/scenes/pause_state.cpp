#include "input/scenes/pause_state.hpp"
#include <iostream>

void PauseState::update(InputState state, Router* router) {
    returnButton = checkButtonCollision(
        state, 
        111, 
        150, 
        35, 
        27
    );

    if(returnButton == ButtonState::ACTIVE) {
        fprintf(stderr, "Exit button clicked\n");
        router->goTo(Scene::Playing);
    }

    if(returnButton == ButtonState::HOVER) {
        // fprintf(stderr, "Exit button hovered\n");
    }
}