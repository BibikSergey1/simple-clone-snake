#pragma once

#include <vector>
#include <memory>

struct Item
{
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    int dx = 0;
    int dy = 0;
};

class Snake
{
public:
    Snake(int headX, int headY, int directionX, int directionY, std::size_t maxCells);

    void move();
    void checkBoundsGameField(int gameFieldX, int gameFieldY,
                              int gameFieldWidth, int gameFieldHeight);

    bool isBitYourself();
    void removeSnake();// умирает
    void createSnake();// рождается
    void changeDirectionHead(int dx, int dy);
    void addSnakeItem();

    std::vector<std::unique_ptr<Item>> items;

    // Защита от быстрой смены направления, которая может привести к столкновению с телом.
    // Пример: при движении влево быстрое нажатие вниз→вправо может создать ситуацию,
    // когда змея развернется и врежется в собственное тело.
    bool canChangeDirection;

private:
    void create(int xHead, int yHead, int directionX, int directionY, int snakeItemSize, int countSnakeItem);

    int headX;
    int headY;
    int directionX;
    int directionY;
    int snakeItemSize = 16;
    int countSnakeItem = 3;
};
