#include "Game.h"
#include "Snake.h"
#include "Foods.h"
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
    if ((snake->items.front()->dy > 0 || snake->items.front()->dy < 0) && snake->testKeys)
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
    else if ((snake->items.front()->dx > 0 || snake->items.front()->dx < 0) && snake->testKeys)
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
    // Проверка еды
    for (const auto& food : foods->foodItems)
    {
        int head_cx = snake->items.front()->x + (snake->items.front()->w / 2),  // computer center of head
            head_cy = snake->items.front()->y + (snake->items.front()->h) / 2;

        // test for collision with food
        if ((head_cx >= food->x && head_cx <= food->x + food->w) &&
            (head_cy >= food->y && head_cy <= food->y + food->h))
        {
            //и растет
            snake->addSnakeItem();
            isNewSnakeItem = true;

            // проверка, чтоб еду не бросить на змею
            int i, food_x, food_y;
            int sizeSnake = snake->items.size();

            do
            {
                food_x = foods->random(1, gameFieldCols - 1) * cellSize;
                food_y = foods->random(1, gameFieldRows - 1) * cellSize;

                for (i = 0; i < sizeSnake; ++i)
                {
                    if (food_x == snake->items.at(i)->x && food_y == snake->items.at(i)->y)
                        break;
                }

                // проверка, чтоб еда не попала на еду.
                for (size_t ii = 0; ii < foods->foodItems.size(); ++ii)
                {
                    if (food_x == foods->foodItems[ii]->x && food_y == foods->foodItems[ii]->y)
                    {
                        if (i >= sizeSnake)
                        {
                            i = 0;
                        }
                        break;
                    }
                }

                if (sizeSnake >= (maxCells - countFoods + 1))
                {
                    break;
                }
            }
            while (i < sizeSnake);

            if (sizeSnake < (maxCells - countFoods + 1))
            {
                food->x = food_x; //
                food->y = food_y; //запоминаем позицию еды
            }
            else
            {
                food->x = -cellSize;
                food->y = -cellSize;
            }
        }
    }

    //qDebug() << "======snake->items.capacity = " << snake->items.capacity();
    snake->move();

    snake->checkBoundsGameField(gameFieldX, gameFieldY, gameFieldWidth, gameFieldHeight);
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
