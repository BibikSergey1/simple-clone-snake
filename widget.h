#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QBitmap>
#include <QKeyEvent>
#include <QVector>

#include <thread>
#include <utility>

namespace Ui {
class Widget;
}

class Sprite;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    void GameInit();
    void DrawToTurn(QPainter* painter, int i, int a_end, int a_angle);
    void DrawToDir(QPainter* painter, int i, int a_end, int a_mid);
    void DrawTextPause(QPainter &p);
    int Random(int low, int high);
    void DrawScore(QPainter& p);

private:
    Ui::Widget *ui;
    int m_width_window;
    int m_height_window;
    int m_col;
    int m_row;
    int m_size_cell;

    QPixmap m_pixmap_tileset;
    QPixmap m_pixmap_background;

    QPixmap m_pixmap_head;
    QPixmap m_pixmap_mid_tail;
    QPixmap m_pixmap_angle_tail;
    QPixmap m_pixmap_end_tail;
    QPixmap m_pixmap_blood;
    QPixmap m_pixmap_wall;

    Sprite* m_pSprite_head;
    Sprite* m_pSprite_tail;
    Sprite* m_pSprite_apple;
    Sprite* m_pSprite_blood;

    QVector<Sprite*> snake;

    int timer_id;
    bool m_test_key;

    bool m_tl;
    bool m_tr;
    bool m_bl;
    bool m_br;

    bool m_newItem;
    bool m_pause;
    unsigned int score_;
};

#endif // WIDGET_H
