#pragma once

struct Point
{
    int x;
    int y;

    // Конструктор по умолчанию
    Point();

    // Конструктор с параметрами
    Point(int x, int y);

    // Оператор сравнения для удобства
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
};
