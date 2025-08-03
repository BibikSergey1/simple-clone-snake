#pragma once

#include <memory>

class Snake;
class Foods;

class Game
{
public:
    Game(int cols, int rows, int countFoods);
    ~Game();

    void initGameField(int fieldCols, int fieldRows);

    void setDirection(const int &dir);

    void update();
    void reborn();
    int getXPositionHead(int countColumns);
    int getYPositionHead(int countRows);
    void setSnakePosition(int countColumns, int countRows);
    void setFoodsPosition();

    std::unique_ptr<Snake> snake;
    std::unique_ptr<Foods> foods;

    int delay = 450;

    int countCellsForWin = 0;
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

    enum {DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN};

    int maxCells;
    bool snakeDied = false;

private:
    //void updateHiScores();
    //bool writeHiScores();
    //bool readHiScores();
};
