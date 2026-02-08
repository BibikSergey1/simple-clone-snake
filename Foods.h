#pragma once

#include <vector>
#include <memory>

class Item;

class Foods
{
public:
    Foods(int countFoods, int x, int y, int gameFieldCellSize);
    ~Foods();

    std::vector<std::unique_ptr<Item> > foodItems;
    void createFoods(int countFoods, int x, int y, int gameFieldCellSize);
    int random(int low, int high);
    void setFoods(int x, int y, int cellSize);
};
