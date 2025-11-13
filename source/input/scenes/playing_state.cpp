#include "input/scenes/playing_state.hpp"
#include <iostream>

void PlayingState::update(InputState state, Router* router) {
    pauseButton = checkButtonCollision(
        state, 
        111, 
        150, 
        35, 
        27
    );

    if(pauseButton == ButtonState::ACTIVE) {
        fprintf(stderr, "Pause button clicked\n");
        router->goTo(Scene::Pause);
    }

    if(pauseButton == ButtonState::HOVER) {
        // fprintf(stderr, "Pause button hovered\n");
    }

    ButtonState gameOverDebugZone = checkButtonCollision(
        state, 
        139, 
        82, 
        9, 
        9
    );

    if(gameOverDebugZone == ButtonState::ACTIVE) {
        fprintf(stderr, "Game Over debug zone clicked\n");
        router->goTo(Scene::End);
    }
}