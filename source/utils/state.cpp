#include "state.hpp"
#include <stdio.h>

ButtonState checkButtonCollision(InputState state, int posX, int posY, int sizeX, int sizeY) {

    if(state.touching) {
        bool inside_x_range = state.touch.px > posX && state.touch.px < posX + sizeX;
        bool inside_y_range = state.touch.py > posY && state.touch.py < posY + sizeY;
        
        if(inside_x_range && inside_y_range) {
            return ButtonState::HOVER;
        }
    }
    
    if (state.touchUp) {
        bool inside_x_range = state.lastTouch.px > posX && state.lastTouch.px < posX + sizeX;
        bool inside_y_range = state.lastTouch.py > posY && state.lastTouch.py < posY + sizeY;

        if(inside_x_range && inside_y_range) {
            return ButtonState::ACTIVE;
        }
    }

    return ButtonState::DEFAULT;
}