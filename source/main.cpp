#include "input/input.hpp"
#include "logic/game.hpp"
#include "graphics/graphics.hpp"
#include "sounds/sounds.hpp"
#include "utils/scene.hpp"
#include "utils/router.hpp"
#include <stdio.h>

int main()
{
	// soundEnable();
    consoleDebugInit(DebugDevice_NOCASH);
	fprintf(stderr, "Starting game...\n");
	Router router{Router()};
	fprintf(stderr, "Router initialized to scene: %d\n", router.currentScene);
	Game* game = new Game();
	fprintf(stderr, "Game initialized\n");
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

	while (!shouldClose)
	{
		if (router.currentScene == Scene::End && !game->isPlayerAlive()) {
			delete game;
			game = new Game();
		}

		double deltaTime = 1.0 / 60.0; // Placeholder for actual frame time calculation

		input.update(game, &router);
		if (router.currentScene == Scene::Playing)
		{
			// game->update(deltaTime, input.state);
		}
		// graphics.update(deltaTime, *game, input.state, &router);
		graphics.render(deltaTime, &router, input, *game);

		swiWaitForVBlank();
		graphics.postRender();
		router.update();
	}

	fprintf(stderr, "Exiting main loop...\n");

	delete game; // Clean up

	fprintf(stderr, "Game resources cleaned up. Exiting.\n");

	return 0;
}