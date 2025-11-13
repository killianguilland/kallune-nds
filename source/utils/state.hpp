#pragma once
#include <nds.h>

struct InputState {
    touchPosition touch;
    touchPosition lastTouch;
    bool touching {false};
    bool wasTouching {false};
    bool touchUp {false};
};

enum ButtonState {
    DEFAULT,
    HOVER,
    ACTIVE
};

ButtonState checkButtonCollision(InputState state, int posX, int posY, int sizeX, int sizeY);