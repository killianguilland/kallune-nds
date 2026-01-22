#include "hardware.hpp"

// Nettoie tout pour repartir sur une base saine
void Hardware::reset()
{
    vramDefault(); // Remet les banques par défaut
    oamClear(&oamMain, 0, 0);
    oamClear(&oamSub, 0, 0);
}

void Hardware::setupUILayout()
{
    // reset(); // Toujours reset avant pour éviter les restes des autres scènes

    // 1. Inversion des écrans : Main = BAS, Sub = HAUT
    lcdMainOnBottom();

    // 2. Configuration des Banques
    vramSetPrimaryBanks(VRAM_A_MAIN_BG_0x06000000, // Bank A pour le fond du bas (Main)
                        VRAM_B_MAIN_BG_0x06020000, // Bank B (libre ou pour un 2eme buffer)
                        VRAM_C_SUB_BG_0x06200000,  // Bank C pour le fond du haut (Sub)
                        VRAM_D_LCD);

    vramSetBankE(VRAM_E_MAIN_SPRITE); // Sprites pour le BAS (Main)
    vramSetBankI(VRAM_I_SUB_SPRITE);  // Sprites pour le HAUT (Sub)

    // 3. MAIN SCREEN SETUP (BAS)
    videoSetMode(
        MODE_5_2D | DISPLAY_BG3_ACTIVE | // On n'active QUE le BG3 pour l'instant
        DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D
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

void Hardware::setupGameLayout()
{
    lcdMainOnTop();

    // --- CONFIGURATION MÉMOIRE VIDÉO (VRAM) ---

    // BANQUE A (128 Ko) : Entièrement pour le Rolling Buffer du Terrain (Main Engine)
    vramSetBankA(VRAM_A_MAIN_BG);

    // BANQUE B (128 Ko) : Pour les Sprites du moteur Main (Animaux, Joueur)
    // On prend une grosse banque car tes animations (loup, stag) sont gourmandes.
    vramSetBankB(VRAM_B_MAIN_SPRITE);

    // BANQUE C (128 Ko) : Pour le décor de l'écran du bas (Minimap)
    vramSetBankC(VRAM_C_SUB_BG);

    // BANQUE D (128 Ko) : Pour les Sprites du moteur Sub (UI, Boutons)
    vramSetBankD(VRAM_D_SUB_SPRITE);

    // Banques secondaires pour les palettes ou bonus
    vramSetBankE(VRAM_E_LCD);        // Gardé en réserve
    vramSetBankI(VRAM_I_SUB_SPRITE); // Extension sprites sub

    // --- ACTIVATION DES MOTEURS ---
    // Mode 5 pour avoir accès aux Bitmaps (BG3)
    videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D);
    videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D);

    // --- INITIALISATION DES COUCHES (BACKGROUNDS) ---

    // BG3 Main : Rolling Buffer 256x256 16-bit
    // L'index 0 correspond au début de la VRAM A (0x06000000)
    bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

    // BG3 Sub : Minimap 256x256 16-bit
    // L'index 0 correspond au début de la VRAM C (0x06200000)
    bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

    // --- INITIALISATION OAM (SPRITES) ---
    oamInit(&oamMain, SpriteMapping_1D_128, false); // 128 pour gérer de grandes planches
    oamInit(&oamSub, SpriteMapping_1D_32, false);

    u16 skyColor  = RGB15(115 / 8, 150 / 8, 225 / 8) | BIT(15);
    u32 fillValue = skyColor | (skyColor << 16);

    // On remplit 256 * 256 pixels (soit 128 Ko)
    dmaFillWords(fillValue, (void*)BG_BMP_RAM(0), 256 * 256 * 2);
    dmaFillWords(fillValue, (void*)BG_BMP_RAM_SUB(0), 256 * 256 * 2);
}

void Hardware::fillScreen()
{
    // Dans ton setup de scène, juste après Hardware::setup...()
    u16 grassColor = RGB15(173 / 8, 119 / 8, 87 / 8) | BIT(15);

    // On remplit le buffer avec cette couleur (32 bits = 2 pixels à la fois)
    // On répète la couleur deux fois pour le format 32 bits
    u32 fillValue = grassColor | (grassColor << 16);

    dmaFillWords(fillValue, (void*)BG_BMP_RAM(0), 256 * 192 * 2);
    dmaFillWords(fillValue, (void*)BG_BMP_RAM_SUB(0), 256 * 192 * 2);
}