#include "settings_state.hpp"
#include <stdio.h>

void SettingsState::update(InputState state, Router* router) {
    returnButton = checkButtonCollision(
        state, 
        111, 
        150, 
        35, 
        27
    );

    if (returnButton == ButtonState::ACTIVE) {
        fprintf(stderr, "Settings button clicked\n");
        router->goTo(Scene::Menu);
    }

    if (returnButton == ButtonState::HOVER) {
        // fprintf(stderr, "Settings button hovered\n");
    }
}