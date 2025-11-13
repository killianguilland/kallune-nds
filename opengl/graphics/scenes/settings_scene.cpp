#include "settings_scene.hpp"

SettingsScene::SettingsScene()
{
    // BACKGROUND
    animation = new AnimatedSprite("menu/background-spritesheet.png", 13.84f, 9.0f, 15, 16, 10);
    background = new Sprite("settings/settings-background.png", 4.9f, 9.0f);

    // BUTTONS
    returnButton = new Sprite("settings/return-button.png", 1.55f, 0.99f);
    returnButtonHover = new Sprite("settings/return-button-hover.png", 1.55f, 0.99f);
}

void SettingsScene::update(InputState inputState, Router* router) {
    state.update(inputState);

    if(state.returnButton == ButtonState::ACTIVE) {
        router->goTo(Scene::Menu);
    }
}

void SettingsScene::draw(double deltaTime)
{
    glClearColor(
        173.0f / 255.0f,
        119.0f / 255.0f,
        87.0f / 255.0f,
        1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    // BACKGROUND
    GameEngine.mvMatrixStack.pushMatrix();
        GameEngine.mvMatrixStack.addTranslation(Vector3D(8.0f - 13.84f / 2, 0.0f, 0.0f));
        GameEngine.updateMvMatrix();
        animation->update(deltaTime);
        animation->draw();
    GameEngine.mvMatrixStack.popMatrix();

    GameEngine.mvMatrixStack.pushMatrix();
        GameEngine.mvMatrixStack.addTranslation(Vector3D(-8.0f + 4.9f / 2, 0.0f, 0.0f));
        GameEngine.updateMvMatrix();
        background->draw();
    GameEngine.mvMatrixStack.popMatrix();

    // BUTTONS
    GameEngine.mvMatrixStack.pushMatrix();
        GameEngine.mvMatrixStack.addTranslation(Vector3D(-8.0f + 1.55f / 2 + 1.6f, -3.0f, 0.0f));
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
}