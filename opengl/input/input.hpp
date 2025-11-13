#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <array>

#include "utils/scene.hpp"
// #include "input/scenes/menu_state.hpp"
#include "utils/state.hpp"
#include "utils/router.hpp"

class Input {

private:
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow *window, int button, int action, int mods);
    GLFWwindow* window;

public:
    Input(GLFWwindow* window);
    void update(Router* router);

    static Input* getInput(GLFWwindow* window);

    InputState state {InputState()};
};