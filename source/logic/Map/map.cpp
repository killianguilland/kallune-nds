#include "./map.hpp"
#include <array>

static const std::array<int, 5> waterSettings = {0, 56, 58, 62, 80};
static const std::array<int, 5> sizeSettings  = {24, 56, 72, 90, 120};
static const std::array<int, 5> typeSettings  = {1, 2, 4, 6, 12};

Map::Map(int waterValue, int sizeValue, int typeValue)
    : width(sizeSettings[sizeValue]), height(sizeSettings[sizeValue]),
    // On initialise le générateur ici, avec ses paramètres !
    generator(sizeSettings[sizeValue], sizeSettings[sizeValue])
{
    this->generator.generate(waterSettings[waterValue], 6U, 2, 4, 4);

    auto genMap = this->generator.getMap();
    map.resize(width * height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            map[y * width + x] = genMap[x][y];
        }
    }
}

// void Map::init() {
//     this->generator.generate(60, 6, 2, 4, 4);
//     map = this->generator.getMap();
// }

std::vector<float> Map::getSpeedMap() const
{
    std::vector<float> speedMap(width * height);

    for (int i = 0; i < width * height; ++i)
    {
        speedMap[i] = getSpeed(map[i]);
    }

    return speedMap;
}

float Map::getSpeed(MapType type) const
{
    switch (type)
    {
    case MapType::GRASS:
        return 1.0f;
    case MapType::WALL:
        return 0.0f;
    case MapType::WATER:
        return 0.3f;
    case MapType::SAND:
        return 0.75f;
    case MapType::SOLID_WALL:
        return 0.0f;
    default:
        return 1.0f;
    }
}

bool Map::isWalkable(int tileX, int tileY)
{
    if (tileX < 0 || tileY < 0 || tileX >= width || tileY >= height)
        return false;
    // Fast 1D access
    return getSpeed(map[tileY * width + tileX]) > 0.0f;
}

void Map::removeFlower(int tileX, int tileY)
{
    if (tileX < 0 || tileY < 0 || tileX >= width || tileY >= height)
        return;
    if (map[tileY * width + tileX] == MapType::FLOWER)
    {
        map[tileY * width + tileX] = MapType::GRASS;
    }
}

void Map::changeTile(int tileX, int tileY, MapType newType)
{
    if (tileX < 0 || tileY < 0 || tileX >= width || tileY >= height)
        return;
    map[tileY * width + tileX] = newType;
}