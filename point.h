#ifndef POINT_H
#define POINT_H

#include <unordered_set>
#include <vector>

class Point
{
public:
    Point(){};
    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    };
    int x;
    int y;

    bool operator==(const Point &p) const { return x == p.x && y == p.y; }

    struct HashFunction
    {
        std::size_t operator()(const Point &point) const
        {
            std::size_t xHash = std::hash<int>()(point.x);
            std::size_t yHash = std::hash<int>()(point.y) << 1;
            return xHash ^ yHash;
        }
    };
};

#endif // POINT_H
