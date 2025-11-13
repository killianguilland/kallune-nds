#include "background.hpp"

Background::Background(const void * bitmap, int bitmapLen, bool subScreen, const int DMA_CHANNEL)
{
    dmaCopyHalfWords(DMA_CHANNEL, bitmap, /* This variable is generated for us by grit. */
                     (subScreen ? (u16 *)BG_BMP_RAM_SUB(0) : (u16 *)BG_BMP_RAM(0)),           /* Our address for main background 3 */
                     bitmapLen);           /* This length (in bytes) is generated from grit. */
}