#include "game_scene.hpp"
#include "graphics/hardware.hpp"
#include "input/input.hpp"
#include "input/scenes/playing_state.hpp"

GameScene::GameScene(const Game* game) : minimap(game->map), terrain(), game(game)
{
    Hardware::setupGameLayout();

    backgroundBottom = new Background(gameBottomBitmap, gameBottomBitmapLen, true);

    pauseButton = new Button(
        0, 0,
        pauseButtonPal,
        pauseButtonPalLen,
        pauseButtonTiles,
        pauseButtonTilesLen,
        SpriteSize_32x32,
        113, 150,
        &oamSub,
        SPRITE_PALETTE_SUB
    );
}

void GameScene::draw(const Input& input)
{
    const PlayingState* playingState = input.getPlayingState();
    pauseButton->draw(playingState->pauseButton);

    // Met à jour la position de la caméra pour suivre le joueur
    x = game->getPlayerX();
    y = game->getPlayerY();

    terrain.draw(game->map, x, y);

    // TODO : if badger moved more than a meaningful amount of pixels
    // if(this->badgerMoved) {
    this->minimap.draw(x, y);
    // this->badgerMoved = false;
    // };
}

void GameScene::postRender()
{
    terrain.swapBuffers();
}

GameScene::~GameScene()
{
    delete pauseButton;
    oamClear(&oamMain, 0, 0);
}