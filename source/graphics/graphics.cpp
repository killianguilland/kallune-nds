#include <nds.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include "graphics/graphics.hpp"
#include <iostream>

Graphics::Graphics()
{
    // Power on all 2D engines
    powerOn(POWER_ALL_2D);

    // Put main screen on bottom (like your original code)
    lcdMainOnBottom();

    /*
    *  Map VRAM to display a background on the main and sub screens.
    *
    *  The vramSetPrimaryBanks function takes four arguments, one for each of
    *  the major VRAM banks. We can use it as shorthand for assigning values to
    *  each of the VRAM bank's control registers.
    *
    *  We map banks A and B to main screen  background memory. This gives us
    *  256KB, which is a healthy amount for 16-bit graphics.
    *
    *  We map bank C to sub screen background memory.
    *
    *  We map bank D to LCD. This setting is generally used for when we aren't
    *  using a particular bank.
    *
    *  We map bank E to main screen sprite memory (aka object memory).
    */
    vramSetPrimaryBanks(VRAM_A_MAIN_BG_0x06000000,
                        VRAM_B_MAIN_BG_0x06020000,
                        VRAM_C_SUB_BG_0x06200000,
                        VRAM_D_LCD);

    vramSetBankE(VRAM_E_MAIN_SPRITE);

    // --- MAIN SCREEN SETUP ---
    videoSetMode(
        MODE_5_2D |           // Set the graphics mode to Mode 5
        DISPLAY_BG2_ACTIVE |  // Enable BG2 for display
        DISPLAY_BG3_ACTIVE |  // Enable BG3 for display
        DISPLAY_SPR_ACTIVE |  // Enable sprites for display
        DISPLAY_SPR_1D        // Enable 1D tiled sprites
    );

    
    // BG3: 16-bit 256x256 bitmap (low priority)
    int mainBg3 = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    bgSetPriority(mainBg3, 3);
    
    // BG2: 16-bit 128x128 bitmap (higher priority)
    int mainBg2 = bgInit(2, BgType_Bmp16, BgSize_B16_128x128, 8, 0);
    bgSetPriority(mainBg2, 2);
    
    // Position backgrounds same as your original
    bgSetScroll(mainBg2, -(SCREEN_WIDTH / 2 - 32), -32);
    bgSetScroll(mainBg3, 0, 0);
    
    // BG0: 4-bit 256x256 text background (top priority for UI/debug text)
    // int mainBg0 = bgInit(0, BgType_Text4bpp, BgSize_T_256x256, 16, 4);
    // bgSetPriority(mainBg0, 0); // topmost priority for UI/debug text
   
    // --- SUB SCREEN SETUP ---
    videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);

    int subBg3 = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    bgSetPriority(subBg3, 3);
    bgSetScroll(subBg3, 0, 0);

    // --- CONSOLE SETUP ---
    // Create a console on sub screen BG0 (for text/UI)
    // static PrintConsole mainConsole;
    // consoleInit(&mainConsole,
    //             0,                  // BG0
    //             BgType_Text4bpp,
    //             BgSize_T_256x256,
    //             16,                 // map base
    //             4,                  // tile base
    //             true,               // true = main screen
    //             true);              // load default font

    // // Make sure BG0 is visible and enabled for text
    // bgSetPriority(mainBg0, 0);
    // bgSetScroll(mainBg0, 0, 0);
    // bgUpdate();

    // consoleSelect(&mainConsole);
    fprintf(stderr, "Console on main screen (BG0)\n");

    setupScene(Scene::Menu);
}

void Graphics::setupScene(Scene scene) {
    // Scene has changed, reset OAM
    initOAM(oam);

    switch (scene)
    {
    case Scene::Menu:
        menu_scene = new MenuScene(oam);
        break;
    case Scene::Settings:
        settings_scene = new SettingsScene(oam);
        break;
    case Scene::Playing:
        game_scene = new GameScene(oam);
        break;
    case Scene::End:
        end_scene = new EndScene(oam);
        break;
    case Scene::Pause:
        pause_scene = new PauseScene(oam);
        break;
    }
}

void Graphics::render(double deltaTime, Router *router, Input input, Game game)
{
    // TODO : calculate deltaTime

    if(router->previousScene != router->currentScene) {
        setupScene(router->currentScene);
    }

    switch (router->currentScene)
    {
    case Scene::Menu:
        menu_scene->draw(deltaTime, input.getMenuState());
        break;
    case Scene::Settings:
        settings_scene->draw(deltaTime, input.getSettingsState());
        break;
    case Scene::Playing:
        game_scene->draw(deltaTime, input.getPlayingState());
        break;
    case Scene::End:
        end_scene->draw(deltaTime, input.getEndState());
        break;
    case Scene::Pause:
        pause_scene->draw(deltaTime, input.getPauseState());
        break;
    }
}

void Graphics::postRender()
{
    updateOAM(oam);
}