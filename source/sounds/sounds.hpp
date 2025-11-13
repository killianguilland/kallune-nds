#pragma once
#include <maxmod9.h>
// #include "soundbank_bin.h"
// #include "soundbank.h"

class Sounds {
public:
    Sounds() {
        // --- Initialize Maxmod ---
        // mmInitDefaultMem((mm_addr)soundbank_bin);
        // mmLoad(SFX_BGM);        // "BGM" will match your filename (bgm.wav -> MOD_BGM)
    }

    void playMusic() {
        // mmStart(SFX_BGM, MM_PLAY_LOOP);
    }

    void pauseMusic() {
        mmPause();
    }
};