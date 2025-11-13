#pragma once
#include <vector>

enum MapType {
  WATER,
  SAND,
  GRASS,
  WALL,
  FLOWER,
  SOLID_WALL
};

enum CornerType {
  TOP,
  BOTTOM,
  LEFT,
  RIGHT,
  TOP_LEFT,
  TOP_RIGHT,
  BOTTOM_LEFT,
  BOTTOM_RIGHT,
  NORMAL,
  SURROUNDED
};

CornerType getCornerType(std::vector<std::vector<MapType>>& map, int x, int y);