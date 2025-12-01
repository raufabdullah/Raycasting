#include "Map.h"

Map::Map(): mapWidth(24), mapHeight(24) {}

Map::Map(int h, int w) : mapWidth(w), mapHeight(h) {}

std::vector<std::vector<int>>& Map::getGrid() { return grid; }

void Map::updateMap(int x, int y, int value) {
    if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
        grid[y][x] = value; 
    }
}