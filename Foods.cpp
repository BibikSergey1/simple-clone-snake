#include "Foods.h"
#include "Snake.h"
#include <time.h>

Foods::Foods(int countFoods, int x, int y, int gameFieldCellSize)
{
    createFoods(countFoods, x, y, gameFieldCellSize);
}

Foods::~Foods()
{}

void Foods::createFoods(int countFoods, int x, int y, int gameFieldCellSize)
{
    for (int ii = 0; ii < countFoods; ++ii)
    {
        auto food = new Item();
        food->x = x * gameFieldCellSize;
        food->y = y * gameFieldCellSize;
        food->w = gameFieldCellSize;
        food->h = gameFieldCellSize;
        foodItems.emplace_back(food);
    }
}

int Foods::random(int low, int high)
{
    return low + rand() % ((high + 1) - low);
}

void Foods::setFoodsRandomly(int x, int y, int cellSize)
{
    for (auto &food : foodItems)
    {
        food->x = x * cellSize;
        food->y = y * cellSize;
    }
}
