#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include "Game.h"

class Item;
class Sprite;

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(int cols, int rows, int countFoods, QWidget *parent = nullptr);
    ~GameWidget();

    void startGame();
    void setGameSettings(int cols, int rows, int countFoods, int delay);
    std::unique_ptr<Game> game;

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void timerEvent(QTimerEvent *);
    void leaveEvent(QEvent *event);
    void enterEvent(QEvent *event);

private:

    void drawSnake(QPainter &painter);
    void drawHead(QPainter &painter, bool isDied) const;
    //void drawBody(QPainter &painter, const size_t &index);
    void drawBody(QPainter &painter, size_t index);
    void drawTail(QPainter &painter, const std::unique_ptr<Item> &segment, int x, int y, int w, int h);
    void drawStraightBody(QPainter &painter, const std::unique_ptr<Item> &segment, int x, int y, int w, int h);
    void drawTurnBody(QPainter &painter, const std::unique_ptr<Item> &current, const std::unique_ptr<Item> &next);
    void drawGreed(QPainter &painter, bool isGreed = true);
    void drawFoods(QPainter &painter);
    void drawGameFiled(QPainter &painter);
    void createFoodSprites();

    void handlePauseState();
    void updateGameState();
    void checkGameConditions();
    bool checkSnakeDeath() const;
    bool checkWinCondition() const;
    void handleGameOver();
    void handleWin();
    void showMessage(const QString &message);
    void restartGame();

    QPixmap pixSnakeLeftHead;
    QPixmap pixSnakeRightHead;
    QPixmap pixSnakeUpHead;
    QPixmap pixSnakeDownHead;

    QPixmap pixSnakeDiedLeftHead;
    QPixmap pixSnakeDiedRightHead;
    QPixmap pixSnakeDiedUpHead;
    QPixmap pixSnakeDiedDownHead;

    QPixmap pixSnakeLeftTail;
    QPixmap pixSnakeRightTail;
    QPixmap pixSnakeUpTail;
    QPixmap pixSnakeDownTail;

    QPixmap pixSnakeHorizBody;
    QPixmap pixSnakeVertBody;

    QPixmap pixSnakeBodyLeftUpTurn;
    QPixmap pixSnakeBodyLeftDownTurn;
    QPixmap pixSnakeBodyRightUpTurn;
    QPixmap pixSnakeBodyRightDownTurn;

    QPixmap pixGameField;
    QPixmap pixFood;
    std::vector<std::unique_ptr<Sprite>> foodSprites;

    int pixSize = 16;
    int timerId = -1;

signals:
    void signalStopGame();
};

#endif // GAMEWIDGET_H
