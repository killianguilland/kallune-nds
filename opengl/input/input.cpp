#include "input.hpp"
#include <iostream>

// Remove the global variable and add keyStates as a member of Input class.
// To access Input instance from static callbacks, use glfwSetWindowUserPointer.

Input* Input::getInput(GLFWwindow* window) {
    return static_cast<Input*>(glfwGetWindowUserPointer(window));
}

void Input::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    Input* input = getInput(window);
    const char* keyName = glfwGetKeyName(key, scancode);
    const char* actionName = nullptr;

    // switch (action) {
    //     case GLFW_PRESS: actionName = "PRESS"; break;
    //     case GLFW_RELEASE: actionName = "RELEASE"; break;
    //     case GLFW_REPEAT: actionName = "REPEAT"; break;
    //     default: actionName = "UNKNOWN"; break;
    // }

    // if (keyName)
    //     std::cout << keyName << " (" << key << ") > " << actionName << std::endl;
    // else
    //     std::cout << key << " > " << actionName << std::endl;
        
    if (input) {
        input->state.keyStates[key] = action;
    }
}

void Input::mouseCallback(GLFWwindow *window, int button, int action, int mods) {
    Input* input = getInput(window);

    if (input) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            input->state.cursorClicked = true;
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        {
            input->state.cursorClicked = false;
            input->state.cursorClickTime = glfwGetTime();
        }
    }
}

void Input::update(Router* router) {
    /*Get the mouse position*/
    glfwGetCursorPos(window, &state.mouseX, &state.mouseY);
    
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwGetFramebufferSize(window, &width, &height);

    float viewWidth {16.0f};
    float viewHeight {9.0f};

    state.x_world = (state.mouseX / width) * viewWidth * 2 - viewWidth / 2.0f;
    state.y_world = (state.mouseY / height) * viewHeight * -2 + viewHeight / 2.0f;

    /* Poll for and process events */
    glfwPollEvents();

    // switch (currentScene)
    // {
    // case Scene::Menu:
    //     menu_state->update(state);
    //     break;
    // case Scene::Playing:
    //     game_scene->update();
    //     break;
    // case Scene::End:
    //     end_scene->update();
    //     break;
    // }
}

Input::Input(GLFWwindow* window) {
    this->window = window;
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);
}