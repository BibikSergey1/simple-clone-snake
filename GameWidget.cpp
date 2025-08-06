#include "GameWidget.h"
#include <QPainter>
#include <QKeyEvent>
#include "Snake.h"
#include "Foods.h"
#include "Sprite.h"
#include <QMessageBox>
#include <QPixmap>
#include <QBitmap>
#include <QApplication>
#include <QDebug>
#include <QGridLayout>

GameWidget::GameWidget(int cols, int rows, int countFoods, QWidget *parent)
    : QWidget{parent}
{
    game = std::make_unique<Game>(cols, rows, countFoods);

    QPixmap pixTileset;
    pixTileset.load(QStringLiteral(":/images/tileset.png"));
    pixTileset.setMask(pixTileset.createMaskFromColor(QColor(255, 0, 255)));

    QPixmap pixSnakeHead = pixTileset.copy(0, 0, pixSize, pixSize);
    pixSnakeHead.scaled(game->cellSize, game->cellSize);
    QTransform trnsf;
    pixSnakeLeftHead = pixSnakeHead.transformed(trnsf.rotate(0));
    pixSnakeRightHead = pixSnakeHead.transformed(trnsf.rotate(180));
    pixSnakeUpHead = pixSnakeHead.transformed(trnsf.rotate(-90));
    pixSnakeDownHead = pixSnakeHead.transformed(trnsf.rotate(-180));

    trnsf.reset();
    QPixmap pixSnakeDiedHead = pixTileset.copy(pixSize * 8, 0, pixSize, pixSize);
    pixSnakeDiedHead.scaled(game->cellSize, game->cellSize);
    pixSnakeDiedLeftHead = pixSnakeDiedHead.transformed(trnsf.rotate(0));
    pixSnakeDiedRightHead = pixSnakeDiedHead.transformed(trnsf.rotate(180));
    pixSnakeDiedUpHead = pixSnakeDiedHead.transformed(trnsf.rotate(-90));
    pixSnakeDiedDownHead = pixSnakeDiedHead.transformed(trnsf.rotate(-180));

    trnsf.reset();
    QPixmap pixSnakeTail = pixTileset.copy(pixSize * 3, 0, pixSize, pixSize);
    pixSnakeTail.scaled(game->cellSize, game->cellSize);
    pixSnakeLeftTail = pixSnakeTail.transformed(trnsf.rotate(90));
    pixSnakeRightTail = pixSnakeTail.transformed(trnsf.rotate(180));
    pixSnakeUpTail = pixSnakeTail.transformed(trnsf.rotate(-90));
    pixSnakeDownTail = pixSnakeTail.transformed(trnsf.rotate(-180));

    trnsf.reset();
    QPixmap pixSnakeBody = pixTileset.copy(pixSize, 0, pixSize, pixSize);
    pixSnakeBody.scaled(game->cellSize, game->cellSize);
    pixSnakeHorizBody = pixSnakeBody;
    pixSnakeVertBody = pixSnakeBody.transformed(trnsf.rotate(-90));

    trnsf.reset();
    QPixmap pixSnakeBodyTurn = pixTileset.copy(pixSize * 2, 0, pixSize, pixSize);
    pixSnakeBodyTurn.scaled(game->cellSize, game->cellSize);
    pixSnakeBodyLeftUpTurn = pixSnakeBodyTurn.transformed(trnsf.rotate(90));
    pixSnakeBodyLeftDownTurn = pixSnakeBodyTurn.transformed(trnsf.rotate(90));
    pixSnakeBodyRightUpTurn = pixSnakeBodyTurn.transformed(trnsf.rotate(180));
    pixSnakeBodyRightDownTurn = pixSnakeBodyTurn.transformed(trnsf.rotate(-90));

    pixGameField = pixTileset.copy(0, pixSize, game->gameFieldWidth, game->gameFieldHeight);
    // pixFood = pixTileset.copy(pixSize * 4, 0, pixSize, pixSize);
    // pixFood.scaled(game->cellSize, game->cellSize);
    pixFood = pixTileset.copy(pixSize * 9, 0, pixSize * 4, pixSize);
    pixFood.scaled(game->cellSize * 4, game->cellSize);
    foodSprite = std::make_unique<Sprite>(pixFood, 4);

    qApp->setOverrideCursor(Qt::ArrowCursor);
}

GameWidget::~GameWidget()
{}

void GameWidget::startGame()
{
    timerId = startTimer(game->delay);
    game->setSnakePosition(game->gameFieldCols, game->gameFieldRows);
    game->setFoodsPosition();
}

void GameWidget::setGameSettings(int cols, int rows, int countFoods, int delay)
{
    game->delay = delay;
    game->gameFieldCols = cols;
    game->gameFieldRows = rows;
    game->countFoods = countFoods;
    game->initGameField(cols + 1, rows + 1);
    game->countCellsForWin = game->maxCells;
    if (game->foods->foodItems.size() != static_cast<size_t>(countFoods))
    {
        game->foods->foodItems.clear();
        game->foods->createFoods(countFoods, 1, 1, cols, rows, game->cellSize);
    }
    game->setSnakePosition(cols + 1, rows + 1);
}

void GameWidget::leaveEvent(QEvent *event)
{
    qApp->setOverrideCursor(Qt::ArrowCursor);

    QWidget::leaveEvent(event);
}

void GameWidget::enterEvent(QEvent *event)
{
    qApp->setOverrideCursor(Qt::BlankCursor);

    QWidget::enterEvent(event);
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Left:
        game->setDirection(Game::DIR_LEFT);
        break;
    case Qt::Key_Right:
        game->setDirection(Game::DIR_RIGHT);
        break;
    case Qt::Key_Up:
        game->setDirection(Game::DIR_UP);
        break;
    case Qt::Key_Down:
        game->setDirection(Game::DIR_DOWN);
        break;
    case Qt::Key_Space:
        game->isPause = !game->isPause;
        break;
    case Qt::Key_Q:
    case 1049:
        killTimer(timerId);
        qApp->setOverrideCursor(Qt::ArrowCursor);
        int ret = QMessageBox::question(nullptr,
                                        QStringLiteral("Information"),
                                        QStringLiteral("Are you sure you want to finish the game?"),
                                        QMessageBox::Ok | QMessageBox::Cancel);

        if (ret == QMessageBox::Ok)
        {
            emit signalStopGame();
            game->reborn();
        }
        else
        {
            timerId = startTimer(game->delay);
        }
        break;
    }
}

void GameWidget::timerEvent(QTimerEvent *e)
{
    if(timerId != e->timerId())
        return;

    if (game->isPause)
    {
        update();
        return;
    }

    game->update();

    for (size_t ii = 0; ii < game->foods->foodItems.size(); ++ii)
    {
        foodSprite->updateFrame(ii);
    }

    game->snakeDied = game->snake->isBitYourself();
    if (game->snakeDied)
    {
        qApp->setOverrideCursor(Qt::ArrowCursor);
        QMessageBox::information(nullptr, QStringLiteral("Information"), QStringLiteral("Game Over"));
        game->reborn();
        game->snakeDied = false;

        emit signalStopGame();
        return;
    }

    if (static_cast<int>(game->snake->items.size()) == game->countCellsForWin)
    {
        qApp->setOverrideCursor(Qt::ArrowCursor);
        QMessageBox::information(nullptr, QStringLiteral("Information"), QStringLiteral("You WIN!!!"));
        game->reborn();

        emit signalStopGame();
        return;
    }

    update();
}

void GameWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    int side = qMin(width(), height());
    painter.setViewport((width() - side) / 2, (height() - side) / 2,
                        side, side);
    painter.setWindow(0, 0, (game->cellSize + 1) * game->gameFieldCols, (game->cellSize + 1) * game->gameFieldRows);

    drawGameFiled(painter);
    drawGreed(painter, true);

    painter.setPen(Qt::green);
    painter.drawText(17, 12, QStringLiteral("Curr Len: %1").arg(QString::number(game->snake->items.size())));
    painter.drawText(90, 12, QStringLiteral("Win: %1").arg(QString::number(game->countCellsForWin)));
    //painter.drawText(145, 12, QStringLiteral("Hi Score: %1").arg(QString::number(game->highestScore)));
    // if (game->snake->items.front()->dx < 0)
    //     painter.drawText(195, 12, QStringLiteral("%1").arg("L"));
    // if (game->snake->items.front()->dx > 0)
    //     painter.drawText(195, 12, QStringLiteral("%1").arg("R"));
    // if (game->snake->items.front()->dy < 0)
    //     painter.drawText(195, 12, QStringLiteral("%1").arg("U"));
    // if (game->snake->items.front()->dy > 0)
    //     painter.drawText(195, 12, QStringLiteral("%1").arg("D"));

    drawFoods(painter);
    drawSnake(painter);

    if (game->isPause)
    {
        painter.setPen(Qt::darkGreen);
        QFont font;
        font.setPixelSize(20);
        painter.setFont(font);
        painter.drawText(game->gameFieldWidth / 2 - 30, game->gameFieldHeight / 2, "P A U S E");
    }
}

void GameWidget::drawGreed(QPainter &painter, bool isGreed)
{
    // Рисуем сетку или крайние поля
    painter.setPen(Qt::green);
    for (int row = game->gameFieldY; row <= game->gameFieldHeight; ++row)
    {
        if (!isGreed)
        {
            if (row == game->gameFieldY || row == game->gameFieldHeight)
                painter.drawLine(game->gameFieldX, row, game->gameFieldWidth, row);
        }
        else
        {
            painter.drawLine(game->gameFieldX, row, game->gameFieldWidth, row);
        }
        row += game->cellSize - 1;
    }

    for (int col = game->gameFieldX; col <= game->gameFieldWidth; ++col)
    {
        if (!isGreed)
        {
            if (!isGreed && (col == game->gameFieldX || col == game->gameFieldWidth))
                painter.drawLine(col, game->gameFieldY, col, game->gameFieldHeight);
        }
        else
        {
            painter.drawLine(col, game->gameFieldY, col, game->gameFieldHeight);
        }
        col += game->cellSize - 1;
    }
}

void GameWidget::drawHead(QPainter &painter, bool isDied) const
{
    const auto &snakeItems = game->snake->items;

    if (snakeItems.front()->dx < 0)
    {
        painter.drawPixmap(snakeItems.front()->x, snakeItems.front()->y,
                           snakeItems.front()->w, snakeItems.front()->h,
                           (!isDied) ? pixSnakeLeftHead : pixSnakeDiedLeftHead);//left
    }
    else if (snakeItems.front()->dx > 0)
    {
        painter.drawPixmap(snakeItems.front()->x, snakeItems.front()->y,
                           snakeItems.front()->w, snakeItems.front()->h,
                           (!isDied) ? pixSnakeRightHead : pixSnakeDiedRightHead);//right
    }
    else if (snakeItems.front()->dy < 0)
    {
        painter.drawPixmap(snakeItems.front()->x, snakeItems.front()->y,
                           snakeItems.front()->w, snakeItems.front()->h,
                           (!isDied) ? pixSnakeUpHead : pixSnakeDiedUpHead);//up
    }
    else if (snakeItems.front()->dy > 0)
    {
        painter.drawPixmap(snakeItems.front()->x, snakeItems.front()->y,
                           snakeItems.front()->w, snakeItems.front()->h,
                           (!isDied) ? pixSnakeDownHead : pixSnakeDiedDownHead);//down
    }
}

void GameWidget::drawBody(QPainter &painter, const size_t &index)
{
    const auto &snake = game->snake->items;

    if (snake.at(index) == snake.back())
    {
        // Поворачиваем хвост

        if (snake.back()->dx < 0)
        {
            painter.drawPixmap(snake.back()->x, snake.back()->y,
                               snake.back()->w, snake.back()->h,
                               pixSnakeLeftTail);//left
        }
        else if (snake.back()->dx > 0)
        {
            painter.drawPixmap(snake.back()->x, snake.back()->y,
                               snake.back()->w, snake.back()->h,
                               pixSnakeRightTail);//right
        }
        else if (snake.back()->dy < 0)
        {
            painter.drawPixmap(snake.back()->x, snake.back()->y,
                               snake.back()->w, snake.back()->h,
                               pixSnakeUpTail);//up
        }
        else if (snake.back()->dy > 0)
        {
            painter.drawPixmap(snake.back()->x, snake.back()->y,
                               snake.back()->w, snake.back()->h,
                               pixSnakeDownTail);//down
        }
    }
    else if (snake.at(index)->dx == snake.at(index + 1)->dx
             || snake.at(index)->dy == snake.at(index + 1)->dy)
    {
        // Движение прямо по горизонтали или вертикали

        if (snake.at(index)->dx < 0 || snake.at(index)->dx > 0)
        {
            painter.drawPixmap(snake.at(index)->x, snake.at(index)->y,
                               snake.at(index)->w, snake.at(index)->h,
                               pixSnakeHorizBody);//Horiz
        }
        else if (snake.at(index)->dy < 0 || snake.at(index)->dy > 0)
        {
            painter.drawPixmap(snake.at(index)->x, snake.at(index)->y,
                               snake.at(index)->w, snake.at(index)->h,
                               pixSnakeVertBody);//Vert
        }

        game->isNewSnakeItem = false;
    }
    else
    {
        // Поворачиваем тело змеи при поворотах.

        if (snake.at(index)->dy < 0 && snake.at(index + 1)->dx < 0)
        {
            // влево и вверх
            painter.drawPixmap(snake.at(index)->x, snake.at(index)->y,
                               snake.at(index)->w, snake.at(index)->h,
                               pixSnakeBodyLeftUpTurn);
        }
        else if (snake.at(index)->dy > 0 && snake.at(index + 1)->dx < 0)
        {
            // влево и вниз
            painter.drawPixmap(snake.at(index)->x, snake.at(index)->y,
                               snake.at(index)->w, snake.at(index)->h,
                               pixSnakeBodyLeftDownTurn);
        }
        else if (snake.at(index)->dy < 0 && snake.at(index + 1)->dx > 0)
        {
            // вправо и вверх
            painter.drawPixmap(snake.at(index)->x, snake.at(index)->y,
                               snake.at(index)->w, snake.at(index)->h,
                               pixSnakeBodyRightUpTurn);
        }
        else if (snake.at(index)->dy > 0 && snake.at(index + 1)->dx > 0)
        {
            // вправо и вниз
            painter.drawPixmap(snake.at(index)->x, snake.at(index)->y,
                               snake.at(index)->w, snake.at(index)->h,
                               pixSnakeBodyRightDownTurn);
        }
        else if (snake.at(index)->dx < 0 && snake.at(index + 1)->dy < 0)
        {
            // вверх и влево
            painter.drawPixmap(snake.at(index)->x, snake.at(index)->y,
                               snake.at(index)->w, snake.at(index)->h,
                               pixSnakeBodyRightDownTurn);
        }
        else if (snake.at(index)->dx > 0 && snake.at(index + 1)->dy < 0)
        {
            // вверх и вправо
            painter.drawPixmap(snake.at(index)->x, snake.at(index)->y,
                               snake.at(index)->w, snake.at(index)->h,
                               pixSnakeBodyLeftDownTurn);
        }
        else if (snake.at(index)->dx < 0 && snake.at(index + 1)->dy > 0)
        {
            // вниз и влево
            painter.drawPixmap(snake.at(index)->x, snake.at(index)->y,
                               snake.at(index)->w, snake.at(index)->h,
                               pixSnakeBodyRightUpTurn);
        }
        else if (snake.at(index)->dx > 0 && snake.at(index + 1)->dy > 0)
        {
            // вниз и вправо
            painter.drawPixmap(snake.at(index)->x, snake.at(index)->y,
                               snake.at(index)->w, snake.at(index)->h,
                               pixSnakeBodyLeftUpTurn);
        }
    }
}

void GameWidget::drawSnake(QPainter &painter)
{
    auto &snake = game->snake->items;

    // Рисуем голову змеи head
    //    painter.setBrush(Qt::darkRed);
    //    painter.drawRect(snake.front()->x,
    //                     snake.front()->y,
    //                     snake.front()->w,
    //                     snake.front()->h);

    drawHead(painter, false);

    // Рисуем тело змеи
    //painter.setBrush(Qt::red);
    for (size_t ii = snake.size() - 1; ii > 0; --ii)
    {
        //        painter.drawRect(snake.at(ii)->x,
        //                         snake.at(ii)->y,
        //                         snake.at(ii)->w,
        //                         snake.at(ii)->h);

        if (snake.at(ii)->dx < 0)
        {
            drawBody(painter, ii);//left
        }
        else if (snake.at(ii)->dx > 0)
        {
            drawBody(painter, ii);//right
        }
        else if (snake.at(ii)->dy < 0)
        {
            drawBody(painter, ii);//up
        }
        else if (snake.at(ii)->dy > 0)
        {
            drawBody(painter, ii);//down
        }
    }

    if (game->snakeDied)
    {
        drawHead(painter, true);
    }
}

void GameWidget::drawFoods(QPainter &painter)
{
    // Рисуем еду для змеи
    //painter.setBrush(Qt::darkGreen);
    for(size_t ii = 0; ii < game->foods->foodItems.size(); ++ii)
    {
        //painter.drawEllipse(food->x, food->y, food->w, food->h);
        //painter.drawPixmap(food->x, food->y, food->w, food->h, pixFood);
        foodSprite->draw(&painter, game->foods->foodItems[ii]->x, game->foods->foodItems[ii]->y, ii);
    }
}

void GameWidget::drawGameFiled(QPainter &painter)
{
    // painter.setBrush(Qt::black);
    // painter.drawRect(0, 0,
    //                  game->gameFieldWidth + game->cellSize,
    //                  game->gameFieldHeight + game->cellSize);

    painter.drawPixmap(game->cellSize, game->cellSize,
                       game->gameFieldWidth - game->cellSize,
                       game->gameFieldHeight - game->cellSize,
                       pixGameField);
}
