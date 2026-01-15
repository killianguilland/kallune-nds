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
    // Maintenant que generator est correctement créé, on peut l'utiliser
    this->generator.generate(waterSettings[waterValue], 6U, 2, 4, 4);
    this->map = this->generator.getMap();
}

// void Map::init() {
//     this->generator.generate(60, 6, 2, 4, 4);
//     map = this->generator.getMap();
// }

vector<vector<float>> Map::getSpeedMap() const
{
    vector<vector<float>> speedMap(width, vector<float>(height, 1.0f));

    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            speedMap[x][y] = getSpeed(map[x][y]);
        }
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
    float speed = getSpeed(map[tileX][tileY]);
    return speed > 0.0f;
}

void Map::removeFlower(int tileX, int tileY)
{
    if (tileX < 0 || tileY < 0 || tileX >= width || tileY >= height)
        return;
    if (map[tileX][tileY] == MapType::FLOWER)
    {
        map[tileX][tileY] = MapType::GRASS;
    }
}

void Map::changeTile(int tileX, int tileY, MapType newType)
{
    if (tileX < 0 || tileY < 0 || tileX >= width || tileY >= height)
        return;
    map[tileX][tileY] = newType;
}