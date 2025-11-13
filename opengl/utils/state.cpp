#include "state.hpp"

ButtonState checkButtonCollision(InputState state, float posX, float posY, float sizeX, float sizeY) {
    bool inside_x_range = state.x_world > posX - sizeX / 2 && state.x_world < posX + sizeX / 2;
    bool inside_y_range = state.y_world > posY - sizeY / 2 && state.y_world < posY + sizeY / 2;

    if(inside_x_range && inside_y_range) {
        if(state.cursorClicked) {
            return ButtonState::ACTIVE;
        }
        else {
            return ButtonState::HOVER;
        }
    }

    return ButtonState::DEFAULT;
}