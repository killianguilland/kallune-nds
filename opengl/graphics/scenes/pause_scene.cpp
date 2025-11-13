#include "pause_scene.hpp"

PauseScene::PauseScene()
{
    texteSprite = new Sprite("pause/texte_ecran_pause.png", 6.75, 1.275f);
    returnButton = new Sprite("pause/bouton_reprendre.png", 3.31f, 0.99f);
    returnButtonHover = new Sprite("pause/bouton_reprendre_hover.png", 3.31f, 0.99f);
    exitButton = new Sprite("pause/bouton_quitter.png", 3.31f, 0.99f);
    exitButtonHover = new Sprite("pause/bouton_quitter_hover.png", 3.31f, 0.99f);
}

void PauseScene::update(InputState inputState, Router* router) {
    state.update(inputState);

    if(state.returnButton == ButtonState::ACTIVE) {
        router->goTo(Scene::Playing);
    }

    if(state.exitButton == ButtonState::ACTIVE) {
        router->goTo(Scene::Menu);
    }
}

void PauseScene::draw()
{
    glClearColor(
        173.0f / 255.0f,
        119.0f / 255.0f,
        87.0f / 255.0f,
        1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    GameEngine.mvMatrixStack.pushMatrix();
    GameEngine.mvMatrixStack.addTranslation(Vector3D(0.0f, 1.75f, 0.0f));
    GameEngine.updateMvMatrix();
    texteSprite->draw();
    GameEngine.mvMatrixStack.popMatrix();
    
    // BUTTONS
    GameEngine.mvMatrixStack.pushMatrix();
        GameEngine.mvMatrixStack.addTranslation(Vector3D(-2.0f, -0.25f, 0.0f));
        GameEngine.updateMvMatrix();
        switch(state.returnButton) {
            case ButtonState::HOVER:
                returnButtonHover->draw();
                break;
            default:
                returnButton->draw();
                break;
        }
    GameEngine.mvMatrixStack.popMatrix();

    GameEngine.mvMatrixStack.pushMatrix();
        GameEngine.mvMatrixStack.addTranslation(Vector3D(2.0f, -0.25f, 0.0f));
        GameEngine.updateMvMatrix();
        switch(state.exitButton) {
            case ButtonState::HOVER:
                exitButtonHover->draw();
                break;
            default:
                exitButton->draw();
                break;
        }
    GameEngine.mvMatrixStack.popMatrix();
}