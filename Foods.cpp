#include "Foods.h"
#include "Snake.h"
#include <time.h>

Foods::Foods(int countFoods, int gameFieldFirstIndexCol, int gameFieldFirstIndexRow,
             int gameFieldCols, int gameFieldRows, int gameFieldCellSize)
{
    createFoods(countFoods, gameFieldFirstIndexCol, gameFieldFirstIndexRow,
                gameFieldCols, gameFieldRows, gameFieldCellSize);
}

Foods::~Foods()
{}

void Foods::createFoods(int countFoods, int gameFieldFirstIndexCol, int gameFieldFirstIndexRow,
                        int gameFieldCols, int gameFieldRows, int gameFieldCellSize)
{
    srand((unsigned)time(NULL));
    for (int ii = 0; ii < countFoods; ++ii)
    {
        auto food = new Item();
        food->x = random(gameFieldFirstIndexCol, gameFieldCols) * gameFieldCellSize;
        food->y = random(gameFieldFirstIndexRow, gameFieldRows) * gameFieldCellSize;
        food->w = gameFieldCellSize;
        food->h = gameFieldCellSize;
        foodItems.emplace_back(food);
    }
}

int Foods::random(int low, int high)
{
    return low + rand() % ((high + 1) - low);
}

void Foods::setFoodsRandomly(int gameFieldCols, int gameFieldRows, int cellSize)
{
    srand((unsigned)time(NULL));
    for (auto &food : foodItems)
    {
        food->x = random(1, gameFieldCols - 1) * cellSize;
        food->y = random(1, gameFieldRows - 1) * cellSize;
    }
}
