#include "input/scenes/end_state.hpp"
#include <iostream>

void EndState::update(InputState state, Router* router) {
    exitButton = checkButtonCollision(
        state, 
        111, 
        150, 
        35, 
        27
    );

    if(exitButton == ButtonState::ACTIVE) {
        fprintf(stderr, "Exit button clicked\n");
        router->goTo(Scene::Menu);
    }

    if(exitButton == ButtonState::HOVER) {
        // fprintf(stderr, "Exit button hovered\n");
    }
}