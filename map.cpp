#include "map.hpp"

Map::Map(const int width, const int height) : width(width), height(height) {
    passable = std::vector<std::vector<bool>>((unsigned long) width, std::vector<bool>((unsigned long) height, true));
}

bool Map::isPassable(int x, int y) {
    if (isInside(x, y)) {
        return passable[x][y];
    } else {
        return false;
    }
}

bool Map::isInside(int x, int y) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

const int Map::getWidth() const {
    return width;
}

const int Map::getHeight() const {
    return height;
}
