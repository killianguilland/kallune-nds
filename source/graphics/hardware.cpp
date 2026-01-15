#include "hardware.hpp"

// Nettoie tout pour repartir sur une base saine
void Hardware::reset() {
    vramDefault(); // Remet les banques par défaut
    oamClear(&oamMain, 0, 0);
    oamClear(&oamSub, 0, 0);
}

void Hardware::setupUILayout() {
    // reset(); // Toujours reset avant pour éviter les restes des autres scènes
    
    // 1. Inversion des écrans : Main = BAS, Sub = HAUT
    lcdMainOnBottom();

    // 2. Configuration des Banques
    vramSetPrimaryBanks(VRAM_A_MAIN_BG_0x06000000,  // Bank A pour le fond du bas (Main)
                        VRAM_B_MAIN_BG_0x06020000,  // Bank B (libre ou pour un 2eme buffer)
                        VRAM_C_SUB_BG_0x06200000,   // Bank C pour le fond du haut (Sub)
                        VRAM_D_LCD);

    vramSetBankE(VRAM_E_MAIN_SPRITE); // Sprites pour le BAS (Main)
    vramSetBankI(VRAM_I_SUB_SPRITE);  // Sprites pour le HAUT (Sub)

    // 3. MAIN SCREEN SETUP (BAS)
    videoSetMode(
        MODE_5_2D | 
        DISPLAY_BG3_ACTIVE |  // On n'active QUE le BG3 pour l'instant
        DISPLAY_SPR_ACTIVE | 
        DISPLAY_SPR_1D
    );

    // Initialisation du fond (BG3) sur le moteur Main (Bas)
    // Map Base 0 = Bank A
    int mainBg3 = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    bgSetPriority(mainBg3, 3);
    bgSetScroll(mainBg3, 0, 0);
   
    // 4. SUB SCREEN SETUP (HAUT)
    videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D);

    // Initialisation du fond (BG3) sur le moteur Sub (Haut)
    // Map Base 0 = Bank C (car c'est le début de la VRAM Sub)
    int subBg3 = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    bgSetPriority(subBg3, 3);
    bgSetScroll(subBg3, 0, 0);

    // 5. Initialisation des OAM
    oamInit(&oamMain, SpriteMapping_1D_32, false); // Pour le bas
    oamInit(&oamSub, SpriteMapping_1D_32, false);  // Pour le haut

    fillScreen();
}

void Hardware::setupGameLayout() {
    // reset();
    lcdMainOnTop();
    
    // Configuration banques (OK)
    vramSetPrimaryBanks(VRAM_A_MAIN_BG_0x06000000, 
                        VRAM_B_MAIN_BG_0x06020000, 
                        VRAM_C_SUB_BG_0x06200000, 
                        VRAM_D_LCD);

    vramSetBankE(VRAM_E_MAIN_SPRITE);
    vramSetBankI(VRAM_I_SUB_SPRITE);
    
    // Activation des moteurs (OK)
    videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D);
    videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D);

    // --- AJOUTS CRUCIAUX ICI ---
    
    // 1. Initialiser le BG3 du moteur Main (Haut) pour le Terrain
    bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

    // 2. Initialiser le BG3 du moteur Sub (Bas) pour la Minimap/UI
    // Sans cette ligne, l'écran reste noir car le registre BG3CNT_SUB est à zéro
    bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

    // 3. Initialiser les DEUX OAM (Haut et Bas)
    // Même si tu n'utilises pas encore de sprites en haut, l'init évite des bugs
    oamInit(&oamMain, SpriteMapping_1D_32, false);
    oamInit(&oamSub, SpriteMapping_1D_32, false);

    fillScreen();
}

void Hardware::fillScreen() {
    // Dans ton setup de scène, juste après Hardware::setup...()
    u16 grassColor = RGB15(173 / 8, 119 / 8, 87 / 8) | BIT(15);

    // On remplit le buffer avec cette couleur (32 bits = 2 pixels à la fois)
    // On répète la couleur deux fois pour le format 32 bits
    u32 fillValue = grassColor | (grassColor << 16);

    dmaFillWords(fillValue, (void*)BG_BMP_RAM(0), 256 * 192 * 2);
    dmaFillWords(fillValue, (void*)BG_BMP_RAM_SUB(0), 256 * 192 * 2);
}