#include "input/input.hpp"
#include "logic/game.hpp"
#include "graphics/graphics.hpp"
#include "utils/scene.hpp"
#include "utils/router.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main()
{
    Router router{Router()};
    Game* game = new Game();
    Graphics graphics{Graphics()};
    Input input{Input(graphics.window)};

    double lastFrameTime = glfwGetTime();

    while (!graphics.shouldClose())
    {
        if (router.currentScene == Scene::End && !game->isPlayerAlive()) {
            delete game;
            game = new Game();
        }

        double currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        input.update(&router);
        if (router.currentScene == Scene::Playing)
        {
            game->update(deltaTime, input.state);
        }
        graphics.update(deltaTime, *game, input.state, &router);
        graphics.render(deltaTime, &router, input.state, *game);
    }

    delete game; // Clean up
    graphics.close();

    return 0;
}