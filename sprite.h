#pragma once

#include <QPixmap>

class Sprite
{
public:
    Sprite(QPixmap &pixmap, int num_frame);
    ~Sprite();

    void draw(QPainter *painter, int x, int y, int index);
    void updateFrame(int index);

    QPixmap &m_Pixmap;
    float m_currFrame;
    float m_currSlowFrame;
    int m_numFrames;
    int m_animationTimerCount;
};

