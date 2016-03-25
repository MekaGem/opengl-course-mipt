#ifndef MAP_HPP
#define MAP_HPP

#include <vector>

class Map {
private:
    const int width;
    const int height;
    std::vector<std::vector<bool>> passable;

public:
    Map(const int width, const int height);

    bool isPassable(int x, int y);

    bool isInside(int x, int y);

    const int getWidth() const;

    const int getHeight() const;
};


#endif //MAP_HPP
