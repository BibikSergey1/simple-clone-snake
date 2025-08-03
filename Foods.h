#pragma once

#include <vector>
#include <memory>

class Item;

class Foods
{
public:
    Foods(int countFoods, int gameFieldFirstIndexCol, int gameFieldFirstIndexRow,
          int gameFieldCols, int gameFieldRows, int gameFieldCellSize);
    ~Foods();

    std::vector<std::unique_ptr<Item> > foodItems;
    void createFoods(int countFoods, int gameFieldFirstIndexCol, int gameFieldFirstIndexRow,
                     int gameFieldCols, int gameFieldRows, int gameFieldCellSize);
    int random(int low, int high);
    void setFoodsRandomly(int gameFieldCols, int gameFieldRows, int cellSize);
};
