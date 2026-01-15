#include "input/states/pause_state.hpp"
#include <iostream>

void PauseState::update(InputState state, Router* router)
{
    exitButton = checkButtonCollision(
        state,
        77,
        150,
        64,
        27
    );

    if (exitButton == ButtonState::ACTIVE)
    {
        fprintf(stderr, "Play button clicked\n");
        router->goTo(Scene::End);
    }

    // if(playButton == ButtonState::HOVER) {
    //     fprintf(stderr, "Play button hovered\n");
    // }

    returnButton = checkButtonCollision(
        state,
        148,
        150,
        31,
        27
    );

    if (returnButton == ButtonState::ACTIVE)
    {
        fprintf(stderr, "Exit button clicked\n");
        router->goTo(Scene::Playing);
    }

    if (returnButton == ButtonState::HOVER)
    {
        // fprintf(stderr, "Exit button hovered\n");
    }
}