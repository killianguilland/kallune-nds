#include "settings_scene.hpp"

SettingsScene::SettingsScene(OAMTable* oam) {
    this->oam = oam;

    int currentTileOffset = 0;

    returnButton = new Button(
        oam,
        0,
        0,
        okButtonPal,
        okButtonPalLen,
        okButtonTiles,
        okButtonTilesLen,
        32,
        113,
        150,
        &currentTileOffset
    );
}

void SettingsScene::draw(double deltaTime, const SettingsState* settingsState)
{
    returnButton->draw(settingsState->returnButton);
}