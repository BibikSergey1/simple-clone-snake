#include "Sprite.h"
#include <QPainter>

Sprite::Sprite(QPixmap &pixmap, int num_frame)
    : m_Pixmap(pixmap)
    , m_currFrame(0.0f)
    , m_numFrames(num_frame)
{
}

Sprite::~Sprite()
{}

void Sprite::draw(QPainter *painter, int x, int y)
{
    painter->drawTiledPixmap(x, y,
                             m_Pixmap.width() / m_numFrames, m_Pixmap.height(),
                             m_Pixmap,
                             (m_Pixmap.width() / m_numFrames) * static_cast<int>(m_currFrame), 0);
}

void Sprite::updateFrame()
{
    m_currFrame += 1;

    if(m_currFrame > m_numFrames)
    {
        m_currFrame -= m_numFrames;
    }
}
