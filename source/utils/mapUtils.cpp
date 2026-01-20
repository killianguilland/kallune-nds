#include "mapUtils.hpp"
#include <array>
#include <cstdint>

// inline auto getCornerType(std::vector<std::vector<MapType>>& map, int x, int y) -> CornerType
// {
//     const MapType tileType  = map[y][x];
//     const int     mapHeight = map.size();
//     const int     mapWidth  = map[0].size();

//     uint8_t mask = 0;

//     if (y > 0 && map[y - 1][x] != tileType)
//         mask |= 1; // Top
//     if (y < mapHeight - 1 && map[y + 1][x] != tileType)
//         mask |= 2; // Bottom
//     if (x > 0 && map[y][x - 1] != tileType)
//         mask |= 4; // Left
//     if (x < mapWidth - 1 && map[y][x + 1] != tileType)
//         mask |= 8; // Right

//     static const std::array<CornerType, 16> lut = {
//         NORMAL,       // 0000: No borders
//         TOP,          // 0001: Top only
//         BOTTOM,       // 0010: Bottom only
//         BOTTOM,       // 0011: Top + Bottom (invalid corner, default to Bottom or special)
//         LEFT,         // 0100: Left only
//         TOP_LEFT,     // 0101: Top + Left
//         BOTTOM_LEFT,  // 0110: Bottom + Left
//         LEFT,         // 0111: Top + Bottom + Left (default to Left)
//         RIGHT,        // 1000: Right only
//         TOP_RIGHT,    // 1001: Top + Right
//         BOTTOM_RIGHT, // 1010: Bottom + Right
//         RIGHT,        // 1011: Top + Bottom + Right
//         NORMAL,       // 1100: Left + Right (invalid, default to Normal)
//         TOP,          // 1101: Top + Left + Right
//         BOTTOM,       // 1110: Bottom + Left + Right
//         SURROUNDED    // 1111: All sides different
//     };

//     return lut[mask];
// }