#include "background.hpp"

Background::Background(const void* bitmap, int bitmapLen, bool subScreen, const int DMA_CHANNEL)
{
    dmaCopyHalfWords(DMA_CHANNEL, bitmap, (subScreen ? (u16*)BG_BMP_RAM_SUB(0) : (u16*)BG_BMP_RAM(0)), bitmapLen);
}