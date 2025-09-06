#include "Snake.h"
#include <QPainter>
#include <QDebug>

Snake::Snake(int headX, int headY, int directionX, int directionY, std::size_t maxCells)
    : canChangeDirection(false)
    , directionX(directionX)
    , directionY(directionY)
    , headX(headX)
    , headY(headY)
{
    items.reserve(maxCells);// Резервируем сразу все место в памяти, чтобы во время игры не тратились ресурсы на релокацию.
}

void Snake::create(int xHead, int yHead, int directionX, int directionY, int snakeItemSize, int countSnakeItem)
{
    int offsetXSnakeItem = 0;
    int offsetYSnakeItem = 0;

    for (int ii = 0; ii < countSnakeItem; ++ii)
    {
        auto snakeItem = new Item();

        if (directionX < 0 && directionY == 0)
        {
            snakeItem->x = xHead + offsetXSnakeItem;
            offsetXSnakeItem += snakeItemSize;
        }
        else if (directionX > 0 && directionY == 0)
        {
            snakeItem->x = xHead + offsetXSnakeItem;
            offsetXSnakeItem -= snakeItemSize;
        }

        if (directionX == 0 && directionY < 0)
        {
            snakeItem->y = yHead + offsetYSnakeItem;
            offsetYSnakeItem += snakeItemSize;
        }
        else if (directionX == 0 && directionY > 0)
        {
            snakeItem->y = yHead + offsetYSnakeItem;
            offsetYSnakeItem -= snakeItemSize;
        }

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
    canChangeDirection = false;// Запрещаем до следующего движения

    Item &head = *items[HEAD_INDEX];
    head.dx = dx;
    head.dy = dy;
}

void Snake::move()
{
    if (items.empty())
        return; // Защита от пустой змеи

    // Двигаем сегменты с хвоста к голове
    for (int ii = items.size() - 1; ii > HEAD_INDEX; --ii)
    {
        items[ii]->x = items[ii - 1]->x;
        items[ii]->y = items[ii - 1]->y;
        items[ii]->dx = items[ii - 1]->dx;
        items[ii]->dy = items[ii - 1]->dy;
    }

    // Двигаем голову
    Item &head = *items[HEAD_INDEX];
    head.x += head.dx;
    head.y += head.dy;

    canChangeDirection = true; // Разрешаем обработку ввода после движения
}

void Snake::checkBoundsGameField(int gameFieldX, int gameFieldY,
                                 int gameFieldWidth, int gameFieldHeight)
{
    //змея выходит за пределы игрового поля

    if (items.empty())
        return;

    Item &head = *items[HEAD_INDEX];

    // Проверка X-координаты (горизонтальные границы)
    if (head.x < gameFieldX)
    {
        head.x = gameFieldWidth - head.w; // Телепорт справа
    }
    else if (head.x + head.w > gameFieldWidth)
    {
        head.x = gameFieldX; // Телепорт слева
    }

    // Проверка Y-координаты (вертикальные границы) - независимо от X
    if (head.y < gameFieldY)
    {
        head.y = gameFieldHeight - head.h; // Телепорт снизу
    }
    else if (head.y + head.h > gameFieldHeight)
    {
        head.y = gameFieldY; // Телепорт сверху
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
