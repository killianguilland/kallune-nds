#include "router.hpp"
#include <cstdio>

void Router::goTo(Scene scene)
{
    fprintf(stderr, "Transitioning from scene %d to scene %d\n", currentScene, scene);
    currentScene = scene;
}

void Router::update()
{
    previousScene = currentScene;
}