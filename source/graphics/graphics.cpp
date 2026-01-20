#include "graphics.hpp"
#include "array"
#include "graphics/scenes/end_scene.hpp"
#include "graphics/scenes/game_scene.hpp"
#include "graphics/scenes/loading_scene.hpp"
#include "graphics/scenes/menu_scene.hpp"
#include "graphics/scenes/pause_scene.hpp"
#include "graphics/scenes/settings_scene.hpp"

template<typename T>
static auto sceneCreator(const Game* g) -> SceneInterface*
{
    return new T(g);
}

using SceneFactory = SceneInterface* (*)(const Game*);

static const std::array<SceneFactory, Scene::COUNT> sceneRegistry = {
    sceneCreator<MenuScene>,
    sceneCreator<SettingsScene>,
    sceneCreator<LoadingScene>,
    sceneCreator<GameScene>,
    sceneCreator<PauseScene>,
    sceneCreator<EndScene>
};

Graphics::Graphics()
{
    // Power on all 2D engines
    powerOn(POWER_ALL_2D);

    setupScene(nullptr, Scene::Menu);
}

void Graphics::setupScene(const Game* game, Scene type)
{
    // 1. Nettoyage automatique (grâce au destructeur virtuel)
    delete currentScene;

    // 2. Création automatique via le registre
    // On récupère la fonction correspondante et on l'exécute
    SceneFactory factory = sceneRegistry[(int)type];

    if (factory != nullptr)
    {
        currentScene = factory(game);
    }
}

void Graphics::render(Router* router, Input input, const Game* game)
{
    cpuStartTiming(0);

    // Changement de scène automatique
    if (router->previousScene != router->currentScene)
    {
        setupScene(game, router->currentScene);
        router->previousScene = router->currentScene;
    }

    // Draw automatique (Polymorphisme)
    if (currentScene != nullptr)
    {
        currentScene->draw(input);
    }

    u32   cycles = cpuEndTiming();
    float usage  = (cycles / 1120380.0f) * 100.0f;
    if (!(framecount % 60))
        fprintf(stderr, "GRAPHICS BUDGET: %f%%   \n", usage);
    framecount++;
}

void Graphics::postRender(Router* router)
{
    // updateOAM(oam);
    oamUpdate(&oamMain);
    oamUpdate(&oamSub);
    if (router->currentScene == Scene::Playing)
    {
        currentScene->postRender();
    }
}