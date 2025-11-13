#include "menu_state.hpp"
#include <stdio.h>

void MenuState::update(InputState state, Router* router) {
    
    playButton = checkButtonCollision(
        state, 
        77, 
        150, 
        64, 
        27
    );

    if(playButton == ButtonState::ACTIVE) {
        fprintf(stderr, "Play button clicked\n");
        router->goTo(Scene::Playing);
    }

    if(playButton == ButtonState::HOVER) {
        // fprintf(stderr, "Play button hovered\n");
    }

    settingsButton = checkButtonCollision(
        state, 
        148, 
        150, 
        31, 
        27
    );

    if(settingsButton == ButtonState::ACTIVE) {
        fprintf(stderr, "Settings button clicked\n");
        router->goTo(Scene::Settings);
    }

    if(settingsButton == ButtonState::HOVER) {
        // fprintf(stderr, "Settings button hovered\n");
    }
}
