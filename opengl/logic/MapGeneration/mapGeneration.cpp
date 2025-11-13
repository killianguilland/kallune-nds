#include "mapGeneration.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <glm/vec2.hpp>
#include <glm/gtc/noise.hpp>

void MapGeneration::printMap()
{
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            std::cout << map[x][y];
        }
        std::cout << "\n";
    }
}

MapGeneration::MapGeneration(unsigned int width, unsigned int height)
    : width{width}, height{height}
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

int MapGeneration::getValue(unsigned int x, unsigned int y)
{
    return map[x][y];
}

vector<vector<MapType>> MapGeneration::getMap()
{
    vector<vector<MapType>> enumsMap = vector<vector<MapType>>(width, vector<MapType>(height, MapType::GRASS));

    for (unsigned int x = 0; x < width; ++x)
    {
        for (unsigned int y = 0; y < height; ++y)
        {

            switch (map[x][y])
            {
            case 1:
                enumsMap[x][y] = MapType::SAND;
                break;
            case 2:
                enumsMap[x][y] = MapType::WATER;
                break;
            case 3:
                enumsMap[x][y] = MapType::WALL;
                break;
            case 4:
                enumsMap[x][y] = MapType::FLOWER;
                break;
            case 5:
                enumsMap[x][y] = MapType::SOLID_WALL;
                break;
            default:
                enumsMap[x][y] = MapType::GRASS;
                break;
            }
        }
    }

    return enumsMap;
}

void MapGeneration::setWidth(unsigned int width)
{
    this->width = width;
}

unsigned int MapGeneration::getWidth()
{
    return width;
}

void MapGeneration::setHeight(unsigned int height)
{
    this->height = height;
}

unsigned int MapGeneration::getHeight()
{
    return height;
}

/*
    chanceToStartAlive sets how dense the initial grid is with living cells.
    starvationLimit is the lower neighbour limit at which cells start dying.
    overpopLimit is the upper neighbour limit at which cells start dying.
    birthNumber is the number of neighbours that cause a dead cell to become alive.
    numberOfSteps is the number of times we perform the simulation step.
*/
void MapGeneration::generate(int chanceToStartAlive, unsigned int numberOfSteps, unsigned int starvationLimit, unsigned int overpopLimit, unsigned int birthNumber)
{
    srand(static_cast<unsigned>(time(0)));

    map = vector<vector<int>>(width, vector<int>(height, 0));
    for (unsigned int x = 0; x < width; ++x)
    {
        for (unsigned int y = 0; y < height; ++y)
        {
            map[x][y] = (rand() % 100 < chanceToStartAlive) ? 1 : (rand() % 100 < 65) ? 0 : 3;
        }
    }

    for (size_t i = 0; i < numberOfSteps; i++)
    {
        step();
    }

    postProcess();

    printMap();
}

void MapGeneration::postProcess()
{
    for (unsigned int x = 0; x < width; ++x)
    {
        for (unsigned int y = 0; y < height; ++y)
        {
            if (map[x][y] == 0)
            {
                map[x][y] = (rand() % 100 < 99) ? 0 : 4;
            }
        }
    }
}

void MapGeneration::step()
{
    vector<vector<int>> newMap = map;

    for (unsigned int x = 0; x < width; ++x)
    {
        for (unsigned int y = 0; y < height; ++y)
        {
            int filled = countNeighbors(x, y, 1, {1, 2});
            int distantFilled = countNeighbors(x, y, 3, {1, 2});
            int moutainCount = countNeighbors(x, y, 1, {3});
            int distantMoutainCount = countNeighbors(x, y, 3, {3});
            int solidWall = countNeighbors(x, y, 1, {5});
            
            if (distantMoutainCount >= 20 && solidWall <= 7) {
                newMap[x][y] = 5;
            }
            else if (moutainCount >= 3)
            {
                newMap[x][y] = 3;
            } 
            else if
                (distantFilled >= 40)
            {
                newMap[x][y] = 2;
            }
            else if (filled >= 5)
            {
                newMap[x][y] = 1;
            }
            else
            {
                newMap[x][y] = 0;
            }

        }
    }

    map = newMap;
}

int MapGeneration::countNeighbors(int x, int y, int dist, vector<int> valueToSearch)
{
    int count = 0;

    for (int dx = -dist; dx <= dist; ++dx)
    {
        for (int dy = -dist; dy <= dist; ++dy)
        {
            if (dx == 0 && dy == 0)
                continue;

            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < static_cast<int>(width) &&
                ny >= 0 && ny < static_cast<int>(height))
            {
                if (std::find(valueToSearch.begin(), valueToSearch.end(), map[nx][ny]) != valueToSearch.end())
                {
                    ++count;
                }
            }
        }
    }

    return count;
}
