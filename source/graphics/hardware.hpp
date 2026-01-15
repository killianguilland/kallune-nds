#pragma once

#include <nds.h>
#include <stdio.h>

class Hardware {
    public:
        static void setupUILayout();
        static void setupGameLayout();
        static void reset();
        static void fillScreen();
};