#include "Game.h"
#include "Snake.h"
#include "Foods.h"
#include "Point.h"
//#include <fstream>
#include <time.h>
#include <QDebug>

Game::Game(int cols, int rows, int countFoods)
    : countFoods(countFoods)
{
    initGameField(cols, rows);

    snake = std::make_unique<Snake>(getXPositionHead(cols), getYPositionHead(rows),
                                    -cellSize, 0, maxCells);

    foods = std::make_unique<Foods>(countFoods, 1, 1, gameFieldCols - 1, gameFieldRows - 1, cellSize);

    countCellsForWin = maxCells;
}

Game::~Game()
{
}

int Game::getXPositionHead(int countColumns)
{
    return (countColumns % 2 == 0) ? gameFieldWidth / 2 : gameFieldWidth / 2 + (cellSize / 2);

}

int Game::getYPositionHead(int countRows)
{
    return (countRows % 2 == 0) ? gameFieldHeight / 2 : gameFieldHeight / 2 + (cellSize / 2);
}

void Game::setSnakePosition(int countColumns, int countRows)
{
    int offsetXSnakeItem = 0;
    for (size_t ii = 0; ii < snake->items.size(); ++ii)
    {
        snake->items[ii]->x = getXPositionHead(countColumns) + offsetXSnakeItem;
        offsetXSnakeItem += cellSize;
        snake->items[ii]->y = getYPositionHead(countRows);
        snake->items[ii]->dx = -cellSize;
        snake->items[ii]->dy = 0;
    }
}

void Game::setFoodsPosition()
{
    for (size_t ii = 0; ii < foods->foodItems.size(); ++ii)
    {
        foods->foodItems[ii]->x = foods->random(1, gameFieldCols - 1) * cellSize;
        foods->foodItems[ii]->y = foods->random(1, gameFieldRows - 1) * cellSize;
        foods->foodItems[ii]->w = cellSize;
        foods->foodItems[ii]->h = cellSize;
    }
}

void Game::initGameField(int fieldCols, int fieldRows)
{
    gameFieldX = cellSize;
    gameFieldY = cellSize;
    gameFieldCols = fieldCols;
    gameFieldRows = fieldRows;
    gameFieldWidth = gameFieldCols * cellSize;
    gameFieldHeight = gameFieldRows * cellSize;
    maxCells = (gameFieldCols - 1) * (gameFieldRows - 1);
}

void Game::setDirection(const int &dir)
{
    if ((snake->items.front()->dy > 0 || snake->items.front()->dy < 0) && snake->canChangeDirection)
    {
        if (dir == DIR_LEFT)
        {
            snake->changeDirectionHead(-cellSize, 0);
        }
        else if (dir == DIR_RIGHT)
        {
            snake->changeDirectionHead(cellSize, 0);
        }
    }
    else if ((snake->items.front()->dx > 0 || snake->items.front()->dx < 0) && snake->canChangeDirection)
    {
        if (dir == DIR_UP)
        {
            snake->changeDirectionHead(0, -cellSize);
        }
        else if (dir == DIR_DOWN)
        {
            snake->changeDirectionHead(0, cellSize);
        }
    }
}

void Game::update()
{
    checkFoodCollisions();
    //qDebug() << "======snake->items.capacity = " << snake->items.capacity();
    snake->move();
    snake->checkBoundsGameField(gameFieldX, gameFieldY, gameFieldWidth, gameFieldHeight);
}

void Game::checkFoodCollisions()
{
    const auto &head = snake->items.front();
    int head_cx = head->x + (head->w / 2);
    int head_cy = head->y + (head->h / 2);

    for (auto &food : foods->foodItems)
    {
        if (isCollision(head_cx, head_cy, food))
        {
            handleFoodCollision(food);
        }
    }
}

bool Game::isCollision(int head_cx, int head_cy,
                       const std::unique_ptr<Item>& food) const
{
    return (head_cx >= food->x && head_cx <= food->x + food->w) &&
           (head_cy >= food->y && head_cy <= food->y + food->h);
}

void Game::handleFoodCollision(std::unique_ptr<Item> &food)
{
    snake->addSnakeItem();
    isNewSnakeItem = true;

    if (canSpawnMoreFood())
    {
        relocateFood(food);
    }
    else
    {
        hideFood(food);
    }
}

void Game::hideFood(std::unique_ptr<Item> &food)
{
    if (food)
    {
        food->x = HIDDEN_FOOD_X;
        food->y = HIDDEN_FOOD_Y;
    }
}

bool Game::canSpawnMoreFood() const
{
    // Вычисляем количество свободных клеток на игровом поле
    const int totalCells = gameFieldCols * gameFieldRows;
    const int occupiedCells = snake->items.size() + foods->foodItems.size();
    const int freeCells = totalCells - occupiedCells;

    // Минимальное количество свободных клеток для генерации новой еды
    constexpr int MIN_FREE_CELLS = 1;

    // Также проверяем, не достигли ли мы максимального количества клеток змеи
    const bool hasSpaceForSnake = snake->items.size() < (maxCells - foods->foodItems.size() + headTailGap);

    return freeCells >= MIN_FREE_CELLS && hasSpaceForSnake;
}

void Game::relocateFood(std::unique_ptr<Item> &food)
{
    Point newPosition;
    do
    {
        newPosition = generateRandomFoodPosition();
    } while (isFoodPositionInvalid(newPosition, food));

    food->x = newPosition.x;
    food->y = newPosition.y;
}

Point Game::generateRandomFoodPosition() const
{
    const int margin = 1; // Вместо магического числа 1
    return {
        foods->random(margin, gameFieldCols - margin) * cellSize,
        foods->random(margin, gameFieldRows - margin) * cellSize
    };
}

bool Game::isFoodPositionInvalid(const Point &position,
                                 const std::unique_ptr<Item>& currentFood) const
{
    // Проверка на змею
    for (const auto& snakeItem : snake->items)
    {
        if (position.x == snakeItem->x && position.y == snakeItem->y)
            return true;
    }

    // Проверка на другую еду
    for (const auto& otherFood : foods->foodItems)
    {
        if (otherFood.get() != currentFood.get() && // Исключаем текущую еду
            position.x == otherFood->x && position.y == otherFood->y)
            return true;
    }

    return false;
}

void Game::reborn()
{
    foods->setFoodsRandomly(gameFieldCols, gameFieldRows, cellSize);
    //умирает.
    snake->removeSnake();
    // И опять возрождается.
    snake->createSnake();
}

// void Game::updateHiScores()
// {
//     auto lengthSnake = static_cast<int>(snake->items.size());
//     if (lengthSnake > highestScore)
//         highestScore = lengthSnake;
// }

// bool Game::writeHiScores()
// {
//     std::ofstream fsOut("HiScores.dat", std::ios_base::out | std::ios_base::binary);
//     if (fsOut.is_open())
//     {
//         //Запись объекта в бинарный файл
//         fsOut.write((const char*)&highestScore,sizeof(highestScore));
//         fsOut.close();
//     }
//     else
//     {
//         return false;
//     }

//     return true;
// }

// bool Game::readHiScores()
// {
//     std::ifstream fsIn("HiScores.dat", std::ios_base::in | std::ios_base::binary);
//     if(fsIn.is_open())
//     {
//         fsIn.read((char*)&highestScore, sizeof(highestScore));
//         fsIn.close();
//     }
//     else
//     {
//         return false;
//     }

//     return true;
// }
