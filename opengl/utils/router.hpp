#pragma once
#include "utils/scene.hpp"

struct Router {
    Scene currentScene {Scene::Menu};
    void goTo(Scene scene);
};