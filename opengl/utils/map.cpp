#include "map.hpp"

CornerType getCornerType(std::vector<std::vector<MapType>>& map, int x, int y) {
  const MapType tileType = map[y][x];
  bool top =    (x < map[0].size() - 1 && map[y][x+1] != tileType);
  bool bottom = (x > 0 && map[y][x-1] != tileType);
  bool left =   (y < map.size() - 1 && map[y+1][x] != tileType);
  bool right =  (y > 0 && map[y-1][x] != tileType);

  if (top && bottom && left && right) return SURROUNDED;
  if (top && left) return TOP_LEFT;
  if (top && right) return TOP_RIGHT;
  if (bottom && left) return BOTTOM_LEFT;
  if (bottom && right) return BOTTOM_RIGHT;
  if (top) return TOP;
  if (bottom) return BOTTOM;
  if (left) return LEFT;
  if (right) return RIGHT;

  return NORMAL;
}