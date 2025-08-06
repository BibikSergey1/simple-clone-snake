#include "Sprite.h"
#include <QPainter>
#include <QDebug>

Sprite::Sprite(QPixmap &pixmap, int num_frame)
    : m_Pixmap(pixmap)
    , m_currFrame(0.0f)
    , m_currSlowFrame(0.0f)
    , m_numFrames(num_frame)
    , m_animationTimerCount(0)
{
}

Sprite::~Sprite()
{}

void Sprite::draw(QPainter *painter, int x, int y, int index)
{
    float currFrame = 0.0f;
    if (index % 2 == 0)
    {
        currFrame = m_currFrame;
    }
    else
    {
        currFrame = m_currSlowFrame;
    }

    painter->drawTiledPixmap(x, y,
                             m_Pixmap.width() / m_numFrames, m_Pixmap.height(),
                             m_Pixmap,
                             (m_Pixmap.width() / m_numFrames) * static_cast<int>(currFrame), 0);
}

void Sprite::updateFrame(int index)
{
    if (index % 2 == 0)
    {
        m_currFrame += 1;
        if(m_currFrame > m_numFrames)
        {
            m_currFrame -= m_numFrames;
        }
    }
    else
    {
        ++m_animationTimerCount;
        if (m_animationTimerCount % 2 == 0)
        {
            m_currSlowFrame += 0.6;
            if(m_currSlowFrame > m_numFrames)
            {
                m_currSlowFrame -= m_numFrames;
            }
        }
    }
}
