#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <array>

struct InputState {
    std::array<int, GLFW_KEY_LAST> keyStates;
    double mouseX {}, mouseY {};
    double x_world {}, y_world {};
    bool cursorClicked {false};
    double cursorClickTime {0.0};
};

enum ButtonState {
    DEFAULT,
    HOVER,
    ACTIVE
};

ButtonState checkButtonCollision(InputState state, float posX, float posY, float sizeX, float sizeY);