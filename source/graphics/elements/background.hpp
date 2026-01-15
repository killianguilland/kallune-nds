#pragma once
#include <nds.h>

class Background {
private:
    /* data */
public:
    Background(const void* bitmap, int bitmapLen, bool subScreen, int DMA_CHANNEL = 3);
    ~Background() = default;
};
