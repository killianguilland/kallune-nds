#include "end_scene.hpp"

EndScene::EndScene()
{
    logoSprite = new Sprite("end/kallune-logo.png", 2.09f, 2.09f);
    texteSprite = new Sprite("end/game-over-text.png", 4.51f, 1.13f);
    buttonSprite = new Sprite("end/return-button.png", 3.86f, 0.99f);
    hoverButtonSprite = new Sprite("end/return_bouton_hover.png", 3.86f, 0.99f);
}

void EndScene::update(InputState inputState, Router *router)
{
    state.update(inputState);

    if (state.exitButton == ButtonState::ACTIVE)
    {
        router->goTo(Scene::Menu);
    }
}

void EndScene::draw()
{
    glClearColor(
        173.0f / 255.0f,
        119.0f / 255.0f,
        87.0f / 255.0f,
        1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GameEngine.mvMatrixStack.pushMatrix();
    GameEngine.mvMatrixStack.addTranslation(Vector3D(0.0f, 1.75f, 0.0f));
    GameEngine.updateMvMatrix();
    logoSprite->draw();
    GameEngine.mvMatrixStack.popMatrix();

    GameEngine.mvMatrixStack.pushMatrix();
    GameEngine.mvMatrixStack.addTranslation(Vector3D(0.0f, -0.25f, 0.0f));
    GameEngine.updateMvMatrix();
    texteSprite->draw();
    GameEngine.mvMatrixStack.popMatrix();

    GameEngine.mvMatrixStack.pushMatrix();
    GameEngine.mvMatrixStack.addTranslation(Vector3D(0.0f, -3.0f, 0.0f));
    GameEngine.updateMvMatrix();
    switch (state.exitButton)
    {
    case ButtonState::HOVER:
        hoverButtonSprite->draw();
        break;
    default:
        buttonSprite->draw();
        break;
    }
    GameEngine.mvMatrixStack.popMatrix();
}