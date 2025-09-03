#pragma once

#include <memory>

class Snake;
class Foods;
class Item;
class Point;

class Game
{
public:
    Game(int cols, int rows, int countFoods);
    ~Game();

    enum class Direction {LEFT, RIGHT, UP, DOWN, NONE};

    void initGameField(int fieldCols, int fieldRows);
    void setDirection(Direction direction);

    void update();
    void reborn();
    int getXPositionHead(int countColumns);
    int getYPositionHead(int countRows);
    void setSnakePosition(int countColumns, int countRows);
    void setFoodsPosition();
    int random(int low, int high) const;

    std::unique_ptr<Snake> snake;
    std::unique_ptr<Foods> foods;

    int delay = 450;

    size_t countCellsForWin = 0;
    bool isNewSnakeItem = false;
    const int cellSize = 16;

    int gameFieldWidth;
    int gameFieldHeight;

    int gameFieldX = 0;
    int gameFieldY = 0;

    int gameFieldCols = 10;
    int gameFieldRows = 10;

    int startLengthSnake = 3;
    int countFoods = 5;

    //int highestScore = 0;

    bool isPause = false;

    int maxCells;
    bool snakeDied = false;

private:
    void checkFoodCollisions();
    bool isCollision(int head_cx, int head_cy,
                     const std::unique_ptr<Item> &food) const;

    void handleFoodCollision(std::unique_ptr<Item> &food);
    bool canSpawnMoreFood() const;
    void relocateFood(std::unique_ptr<Item> &food);
    Point generateRandomFoodPosition() const;
    bool isFoodPositionInvalid(const Point &position,
                               const std::unique_ptr<Item> &currentFood) const;

    void hideFood(std::unique_ptr<Item> &food);

    static const int HIDDEN_FOOD_X = -9999;
    static const int HIDDEN_FOOD_Y = -9999;
    static const int headTailGap = 1;

    //void updateHiScores();
    //bool writeHiScores();
    //bool readHiScores();
};
