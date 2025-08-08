#pragma once

#include <QPixmap>

class Sprite
{
public:
    Sprite(QPixmap &pixmap, int num_frame);
    ~Sprite();

    void draw(QPainter *painter, int x, int y);
    void updateFrame();

    QPixmap &m_Pixmap;
    float m_currFrame;
    int m_numFrames;
    float frameDelay;
};

