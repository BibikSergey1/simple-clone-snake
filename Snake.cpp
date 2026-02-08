#include "Snake.h"
#include <QPainter>
#include <QDebug>

Snake::Snake(int headX, int headY, int directionX, int directionY, std::size_t maxCells)
    : testKeys(false)
    , headX(headX)
    , headY(headY)
    , directionX(directionX)
    , directionY(directionY)
{
    items.reserve(maxCells);// Резервируем сразу все место в памяти, чтобы во время игры не тратились ресурсы на релокацию.
    createSnake();
}

void Snake::create(int xHead, int yHead, int directionX, int directionY, int snakeItemSize, int countSnakeItem)
{
    int offsetXSnakeItem = 0;
    for (int ii = 0; ii < countSnakeItem; ++ii)
    {
        auto snakeItem = new Item();
        snakeItem->x = xHead + offsetXSnakeItem;
        offsetXSnakeItem += snakeItemSize;
        snakeItem->y = yHead;
        snakeItem->dx = directionX;
        snakeItem->dy = directionY;
        snakeItem->w = snakeItemSize;
        snakeItem->h = snakeItemSize;
        items.emplace_back(snakeItem);
    }
}

void Snake::addSnakeItem()
{
    auto snakeItem = new Item();
    snakeItem->x = items.back()->x;
    snakeItem->y = items.back()->y;
    snakeItem->dx = items.back()->dx;
    snakeItem->dy = items.back()->dy;
    snakeItem->w = items.back()->w;
    snakeItem->h = items.back()->h;

    items.emplace_back(snakeItem);
}

void Snake::changeDirectionHead(int dx, int dy)
{
    testKeys = false;

    items[0]->dx = dx;
    items[0]->dy = dy;
}

void Snake::move()
{
    //змея ползет
    for (int ii = items.size() - 1; ii > 0; --ii)
    {
        items[ii]->x = items[ii - 1]->x;
        items[ii]->y = items[ii - 1]->y;
        items[ii]->dx = items[ii - 1]->dx;
        items[ii]->dy = items[ii - 1]->dy;
    }

    items[0]->x += items[0]->dx;
    items[0]->y += items[0]->dy;

    testKeys = true;
}

void Snake::checkBoundsGameField(const int &gameFieldX, const int &gameFieldY,
                                 const int &gameFieldWidth, const int &gameFieldHeight)
{
    //змея выходит за пределы игрового поля

    // Проверка левой границы по координате X
    if(items[0]->x < gameFieldX)
    {
        items[0]->x = gameFieldWidth - items[0]->w;
        items[0]->y = items[0]->y;
    }
    // Проверка правой границы по координате X
    else if(items[0]->x > gameFieldWidth - items[0]->w)
    {
        items[0]->x = gameFieldX;
        items[0]->y = items[0]->y;
    }
    // Проверка нижней границы по координате Y
    if(items[0]->y > gameFieldHeight - items[0]->h)
    {
        items[0]->x = items[0]->x;
        items[0]->y = gameFieldY;
    }
    // Проверка верхней границы по координате Y
    else if(items[0]->y < gameFieldY)
    {
        items[0]->x = items[0]->x;
        items[0]->y = gameFieldHeight - items[0]->h;
    }
}

bool Snake::isBitYourself()
{
    // Проверка, что змея кусает себя...
    bool res = false;

    if (items.size() <= 4)
        return res;

    int head_cx = items.front()->x + (items.front()->w / 2),  // computer center of head
        head_cy = items.front()->y + (items.front()->h) / 2;

    for (size_t ii = 1; ii < items.size(); ++ii)
    {
        // test for collision with body
        if ((head_cx >= items[ii]->x && head_cx <= items[ii]->x + items[ii]->w) &&
           (head_cy >= items[ii]->y && head_cy <= items[ii]->y + items[ii]->h))
        {
            res = true;
            break;
        }
    }
    return res;
}

void Snake::removeSnake()
{
    items.clear();
}

void Snake::createSnake()
{
    create(headX, headY, directionX, directionY, snakeItemSize, countSnakeItem);
}
