#include "Point.h"

Point::Point()
    : x(0), y(0)
{}

Point::Point(int x, int y)
    : x(x), y(y)
{}

// Оператор сравнения для удобства
bool Point::operator==(const Point& other) const
{
    return x == other.x && y == other.y;
}

bool Point::operator!=(const Point& other) const
{
    return !(*this == other);
}
