#include <cstdio>
#include "graphics/graphics.hpp"
#include "input/input.hpp"
#include "input/states/settings_state.hpp"
#include "logic/game.hpp"
#include "sounds/sounds.hpp"
#include "utils/router.hpp"
#include "utils/scene.hpp"

int main()
{
    // soundEnable();
    consoleDebugInit(DebugDevice_NOCASH);
    fprintf(stderr, "Starting game...\n");
    Router router{Router()};
    fprintf(stderr, "Router initialized to scene: %d\n", router.currentScene);
    Game* game{nullptr};
    // fprintf(stderr, "Game initialized\n");
    Graphics graphics{Graphics()};
    fprintf(stderr, "Graphics initialized\n");
    Input input{Input()};
    fprintf(stderr, "Input initialized\n");
    Sounds sounds{Sounds()};
    fprintf(stderr, "Sounds initialized\n");

    sounds.playMusic();

    // Main game loop
    fprintf(stderr, "Entering main loop...\n");

    bool shouldClose = false;

    // int frames = 0;
    // int fps = 0;

    // Start a timer (Timer 0) at 60Hz frequency to track time
    // timerStart(0, ClockDivider_1024, (u16)TIMER_FREQ_1024(1), NULL);

    while (!shouldClose)
    {
        // frames++;
        if (router.currentScene == Scene::Loading)
        {
            delete game;
            const SettingsState* settings = input.getState<SettingsState>();

            game = new Game(settings->waterValue, settings->sizeValue, settings->typeValue);
            router.goTo(Scene::Playing);
        }

        input.update(&router);
        if (router.currentScene == Scene::Playing)
        {
            game->update(input.state);
            if (game != nullptr && !game->isPlayerAlive())
                router.goTo(Scene::End);
        }
        graphics.render(&router, input, game);

        swiWaitForVBlank();
        graphics.postRender(&router);
        router.update();

        // fflush(stderr);
        // fprintf(stderr, "FPS: %d   \n", fps);
    }

    fprintf(stderr, "Exiting main loop...\n");

    delete game; // Clean up

    fprintf(stderr, "Game resources cleaned up. Exiting.\n");

    return 0;
}