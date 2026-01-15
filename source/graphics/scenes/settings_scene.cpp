#include "settings_scene.hpp"
#include "graphics/hardware.hpp"
#include "input/scenes/settings_state.hpp"

// 1. Table des pointeurs vers les palettes générées par grit
const unsigned short* waterPals[] = {
    waterButton0Pal, waterButton1Pal, waterButton2Pal, waterButton3Pal, waterButton4Pal
};
const unsigned int waterPalsLen[] = {
    waterButton0PalLen, waterButton1PalLen, waterButton2PalLen, waterButton3PalLen, waterButton4PalLen
};

// 2. Table des pointeurs vers les tiles
const unsigned int* waterTiles[] = {
    waterButton0Tiles, waterButton1Tiles, waterButton2Tiles, waterButton3Tiles, waterButton4Tiles
};
const unsigned int waterTilesLen[] = {
    waterButton0TilesLen, waterButton1TilesLen, waterButton2TilesLen, waterButton3TilesLen, waterButton4TilesLen
};

const unsigned short* sizePals[] = {
    sizeButton0Pal, sizeButton1Pal, sizeButton2Pal, sizeButton3Pal, sizeButton4Pal
};
const unsigned int sizePalsLen[] = {
    sizeButton0PalLen, sizeButton1PalLen, sizeButton2PalLen, sizeButton3PalLen, sizeButton4PalLen
};

// 2. Table des pointeurs vers les tiles
const unsigned int* sizeTiles[] = {
    sizeButton0Tiles, sizeButton1Tiles, sizeButton2Tiles, sizeButton3Tiles, sizeButton4Tiles
};
const unsigned int sizeTilesLen[] = {
    sizeButton0TilesLen, sizeButton1TilesLen, sizeButton2TilesLen, sizeButton3TilesLen, sizeButton4TilesLen
};

const unsigned short* typePals[] = {
    typeButton0Pal, typeButton1Pal
};
const unsigned int typePalsLen[] = {
    typeButton0PalLen, typeButton1PalLen
};

// 2. Table des pointeurs vers les tiles
const unsigned int* typeTiles[] = {
    typeButton0Tiles, typeButton1Tiles
};
const unsigned int typeTilesLen[] = {
    typeButton0TilesLen, typeButton1TilesLen
};

SettingsScene::SettingsScene(const Game* /*_*/)
{
    Hardware::setupUILayout();

    backgroundTop    = new Background(settingsTopBitmap, settingsTopBitmapLen, true);
    backgroundBottom = new Background(settingsBottomBitmap, settingsBottomBitmapLen, false);

    returnButton = new Button(
        0,
        0,
        okButtonPal,
        okButtonPalLen,
        okButtonTiles,
        okButtonTilesLen,
        SpriteSize_32x32,
        113,
        150
    );

    waterBtn = new Button(1, 1, waterButton0Pal, waterButton0PalLen, waterButton0Tiles, waterButton0TilesLen, SpriteSize_64x64, 24, 62);
    sizeBtn  = new Button(2, 2, sizeButton0Pal, sizeButton0PalLen, sizeButton0Tiles, sizeButton0TilesLen, SpriteSize_64x64, 96, 62);
    typeBtn  = new Button(3, 3, waterButton0Pal, waterButton0PalLen, waterButton0Tiles, waterButton0TilesLen, SpriteSize_64x64, 168, 62);
}

void SettingsScene::draw(const Input& input)
{
    const SettingsState* settingsState = input.getSettingsState();
    returnButton->draw(settingsState->returnButton);

    if (settingsState->waterValue != lastWater)
    {
        waterBtn->updateGraphics(
            waterTiles[settingsState->waterValue],
            waterTilesLen[settingsState->waterValue],
            waterPals[settingsState->waterValue],
            waterPalsLen[settingsState->waterValue]
        );
        lastWater = settingsState->waterValue;
    }

    if (settingsState->sizeValue != lastSize)
    {
        sizeBtn->updateGraphics(
            sizeTiles[settingsState->sizeValue],
            sizeTilesLen[settingsState->sizeValue],
            sizePals[settingsState->sizeValue],
            sizePalsLen[settingsState->sizeValue]
        );
        lastSize = settingsState->sizeValue;
    }

    if (settingsState->typeValue != lastType)
    {
        typeBtn->updateGraphics(
            typeTiles[settingsState->typeValue],
            typeTilesLen[settingsState->typeValue],
            typePals[settingsState->typeValue],
            typePalsLen[settingsState->typeValue]
        );
        lastType = settingsState->typeValue;
    }

    // On dessine normalement
    waterBtn->draw(settingsState->waterButton);
    sizeBtn->draw(settingsState->sizeButton);
    typeBtn->draw(settingsState->typeButton);
}

void SettingsScene::postRender()
{
}

SettingsScene::~SettingsScene()
{
    delete returnButton;
    delete waterBtn;
    delete sizeBtn;
    delete typeBtn;
    oamClear(&oamMain, 0, 0);
}