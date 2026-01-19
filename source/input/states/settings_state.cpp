#include "settings_state.hpp"
#include <cstdio>

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

    waterButton = checkButtonCollision(
        state, 
        24, 
        62, 
        64, 
        64
    );

    if (waterButton == ButtonState::ACTIVE) {
        fprintf(stderr, "Water button clicked\n");
        this->waterValue = (this->waterValue + 1) % 5;
    }

    sizeButton = checkButtonCollision(
        state, 
        96, 
        62, 
        64, 
        64
    );

    if (sizeButton == ButtonState::ACTIVE) {
        fprintf(stderr, "Size button clicked\n");
        this->sizeValue = (this->sizeValue + 1) % 5;
    }

    typeButton = checkButtonCollision(
        state, 
        168, 
        62, 
        64, 
        64
    );

    if (typeButton == ButtonState::ACTIVE) {
        fprintf(stderr, "Type button clicked\n");
        this->typeValue = (this->typeValue + 1) % 2;
    }
}