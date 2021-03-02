#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include "sprite.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_size_cell = 16;
    m_col = width()/m_size_cell;
    m_row = height()/m_size_cell;
    m_width_window = m_col * m_size_cell;
    m_height_window = m_row * m_size_cell;
    resize(m_width_window,m_height_window);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    m_pixmap_tileset.load(":/images/tileset.png");
    m_pixmap_tileset.setMask(m_pixmap_tileset.createMaskFromColor(QColor(255,0,255)));
    m_pixmap_background = m_pixmap_tileset.copy(0,m_size_cell,m_width_window,m_height_window);

    m_pixmap_blood.load(":/images/blood.png");
    m_pixmap_blood.setMask(m_pixmap_blood.createMaskFromColor(QColor(255,255,255)));

    m_pixmap_wall = m_pixmap_tileset.copy(m_size_cell*8+2,0,m_size_cell,m_size_cell);

    GameInit();

    timer_id = startTimer(140);
    m_test_key = false;
}

Widget::~Widget()
{
    delete ui;
    killTimer(timer_id);
    for(int i = 0;i<snake.size();i++)
    {
        delete snake[i];
        snake[i] = nullptr;
    }

    snake.clear();

    m_foods.clear();

    if (m_pSprite_blood)
    {
        delete m_pSprite_blood;
        m_pSprite_blood = nullptr;
    }
}

void Widget::GameInit()
{
    m_test_key = false;
    m_tl = false;
    m_tr = false;
    m_bl = false;
    m_br = false;
    m_newItem = false;

    m_pixmap_head = m_pixmap_tileset.copy(0,0,m_size_cell,m_size_cell);
    m_pSprite_head = new Sprite(m_pixmap_head, 1, 180);
    m_pSprite_head->SetPosition(m_size_cell, (m_row*m_size_cell)/2);
    m_pSprite_head->SetVelocity(16, 0);
    snake.push_back(m_pSprite_head);

    m_pixmap_mid_tail = m_pixmap_tileset.copy(16,0,m_size_cell,m_size_cell);
    m_pSprite_tail = new Sprite(m_pixmap_mid_tail, 1);
    m_pSprite_tail->SetPosition(m_size_cell - m_pSprite_tail->GetPosition().width(),
                               (m_row*m_size_cell)/2);
    m_pSprite_tail->SetVelocity(16, 0);
    snake.push_back(m_pSprite_tail);

    m_pSprite_tail = new Sprite(m_pixmap_mid_tail, 1);
    m_pSprite_tail->SetPosition(m_size_cell - m_pSprite_tail->GetPosition().width()*2,
                               (m_row*m_size_cell)/2);
    m_pSprite_tail->SetVelocity(16, 0);
    snake.push_back(m_pSprite_tail);

    m_pixmap_angle_tail = m_pixmap_tileset.copy(16*2,0,m_size_cell,m_size_cell);
    m_pixmap_end_tail = m_pixmap_tileset.copy(16*3,0,16,16);

    for (int ii=0; ii < COUNT_FOODS; ++ii)
    {
        Sprite* pSprite_apple = new Sprite(m_pixmap_tileset.copy(16*5,0,m_size_cell*3,m_size_cell), 3);
        pSprite_apple->SetPosition(Random(2, m_col-2)*m_size_cell,
                                     Random(2, m_row-2)*m_size_cell);
        pSprite_apple->SetVelocity(0, 0);

        m_foods.emplace_back(pSprite_apple);
    }

    m_pSprite_blood = new Sprite(m_pixmap_blood.copy(0,0,40*5,0), 5);
    m_pSprite_blood->SetVelocity(0, 0);

    score_ = 0;
    m_pause = false;
}

int Widget::Random(int low, int high)
{
    return low + qrand() % ((high + 1) - low);
}

void Widget::resizeEvent(QResizeEvent *event)
{
    m_col = width()/m_size_cell;
    m_row = height()/m_size_cell;

    m_width_window = m_col * m_size_cell;
    m_height_window = m_row * m_size_cell;
    resize(m_width_window,m_height_window);
}

void Widget::timerEvent(QTimerEvent *e)
{
    if(timer_id == e->timerId())
    {
        update();

        if(m_pause == false)
        {
            //змея ползет
            for(int i = snake.size()-1;i > 0 ;i--)
            {
                snake[i]->SetPosition(snake[i-1]->GetPosition());
                snake[i]->SetVelocity(snake[i-1]->GetVelocity());
            }

            snake.front()->SetPosition(snake.front()->GetPosition().left() + snake.front()->GetVelocity().x(),
                    snake.front()->GetPosition().top() + snake.front()->GetVelocity().y());

            m_test_key = true;
        }

        //змея выходит за пределы игрового поля
        if(snake.front()->GetPosition().left() < m_size_cell)
        {
            snake.front()->SetPosition((m_width_window-m_size_cell) - snake.front()->GetPosition().width(),
                                       snake.front()->GetPosition().top());
        }
        else if(snake.front()->GetPosition().right() > m_width_window-m_size_cell)
            snake.front()->SetPosition(m_size_cell, snake.front()->GetPosition().top());

        if(snake.front()->GetPosition().bottom() > m_height_window-m_size_cell)
            snake.front()->SetPosition(snake.front()->GetPosition().left(), m_size_cell);
        else if(snake.front()->GetPosition().top() < m_size_cell)
        {
            snake.front()->SetPosition(snake.front()->GetPosition().left(),
                                       m_height_window-m_size_cell-snake.front()->GetPosition().height());
        }

        for(const auto& food : m_foods)
        {
            //голова змеи ест яблоко
            if(food->GetPosition().contains(m_pSprite_head->GetPosition()))
            {
                //и растет
                m_newItem = true;
                m_pSprite_tail = new Sprite(m_pixmap_tileset.copy(16,0,m_size_cell,m_size_cell),1);
                m_pSprite_tail->SetPosition(snake[snake.size()-1]->GetPosition());
                m_pSprite_tail->SetVelocity(snake[snake.size()-1]->GetVelocity());
                snake.push_back(m_pSprite_tail);

                m_pSprite_blood->SetPosition(food->GetPosition());
                m_pSprite_blood->SetOneFrame(true);

                score_++;

                //перемещаем яблоко в случайную координату
                food->SetPosition(Random(2, m_col-2)*m_size_cell,
                                             Random(2, m_row-2)*m_size_cell);
            }
        }

        for(const auto& food : m_foods)
        {
            // проверка, если еда попадает на змею,
            // то опять перемещаем в случайную координату
            for(int i = 0; i < snake.size(); i++)
            {
                if(food->GetPosition().contains(snake[i]->GetPosition()))
                {
                    //перемещаем яблоко в случайную координату
                    food->SetPosition(Random(2, m_col-2)*m_size_cell,
                                      Random(2, m_row-2)*m_size_cell);
                    break;
                }
            }
        }

        //змея кусает свой хвост, умирает...
        for(int i = 1; i < snake.size(); i++)
        {
            if(snake.front()->GetPosition().contains(snake[i]->GetPosition()))
            {
               QMessageBox::about(0,"Snake","Game Over");

               for(int i = 0;i<snake.size();i++)
               {
                   delete snake[i];
                   snake[i] = 0;
               }

               snake.clear();
               m_foods.clear();

               // ...и опять возрождается
               GameInit();
            }
        }

        for(const auto& food : m_foods)
            food->UpdateFrame();

        m_pSprite_blood->UpdateFrame();
    }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Left:
        {
            if((snake.front()->GetVelocity().y() > 0 || snake.front()->GetVelocity().y() < 0) && m_test_key)
                snake.front()->SetVelocity(-16,0);
        }break;
        case Qt::Key_Right:
        {
            if((snake.front()->GetVelocity().y() > 0 || snake.front()->GetVelocity().y() < 0) && m_test_key)
                snake.front()->SetVelocity(16,0);
        }break;
        case Qt::Key_Up:
        {
            if((snake.front()->GetVelocity().x() > 0 || snake.front()->GetVelocity().x() < 0) && m_test_key)
                snake.front()->SetVelocity(0,-16);
        }break;
        case Qt::Key_Down:
        {
            if((snake.front()->GetVelocity().x() > 0 || snake.front()->GetVelocity().x() < 0) && m_test_key)
                snake.front()->SetVelocity(0,16);
        }break;
        case Qt::Key_Space:
        {
            m_pause = (m_pause) ? false : true;
        }break;
    }

    m_test_key = false;
}

void Widget::DrawToTurn(QPainter* painter, int i, int a_end, int a_angle)
{
    m_tl = false;
    m_tr = false;
    m_bl = false;
    m_br = false;
    if(i == snake.size()-1)
        snake[i]->Draw(painter, m_pixmap_end_tail, a_end);
    else if(m_newItem)
    {
        m_newItem = false;
        snake[i-1]->Draw(painter, m_pixmap_angle_tail, a_angle);
    }
    else
        snake[i]->Draw(painter, m_pixmap_angle_tail, a_angle);
}

void Widget::DrawToDir(QPainter* painter, int i, int a_end, int a_mid)
{
    if(i == snake.size()-1)
        snake[i]->Draw(painter, m_pixmap_end_tail, a_end);
    else if(m_newItem)
    {
        m_newItem = false;
        if(!m_br || !m_tr)
            snake[i-1]->Draw(painter, m_pixmap_mid_tail, a_mid);
    }
    else
        snake[i]->Draw(painter, m_pixmap_mid_tail, a_mid);
}

void Widget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

//    int side = qMin(width(), height());

//    painter.setViewport((width() - side) / 2, (height() - side) / 2,
//                        side, side);

//    painter.setWindow(0,0,m_size_cell*m_col,m_size_cell*m_row);

    painter.drawPixmap(0,0,m_width_window,m_height_window,m_pixmap_background);

    // рисуем границы периметра
    for(int row=0;row<m_row*m_size_cell;row+=m_size_cell)
    {
        for(int col=0;col<m_col*m_size_cell;col+=m_size_cell)
        {
            if(row == 0 || row == (m_row-1)*m_size_cell ||
                    col == 0 || col == (m_col-1)*m_size_cell)
            {
                // границу где будет нарисовано количество съеденной еды не рисуем.
                if(row==0*m_size_cell && (col>=2*m_size_cell && col<=9*m_size_cell))
                    continue;
                else
                    painter.drawPixmap(col,row,m_pixmap_wall);
            }
        }
    }

    // Рисуем количество съеденной еды
    DrawScore(painter);

    if(m_pause == false)
    {
        // Рисуем еду
        for(const auto& food : m_foods)
            food->Draw(&painter);

        // Рисуем то, что осталось от еды.
        if(m_pSprite_blood->GetOneFrame())
        {
            m_pSprite_blood->Draw(&painter);
            m_pSprite_blood->SetOneFrame(false);
        }
    }
    else
    {
        // Рисуем паузу
        DrawTextPause(painter);
    }

    if(m_pause == false)
    {
        // Рисуем голову змеи head
        if(snake.front()->GetVelocity() == QPoint(0,-16))
            snake.front()->Draw(&painter, m_pixmap_head, 90);
        else if(snake.front()->GetVelocity() == QPoint(0,16))
            snake.front()->Draw(&painter, m_pixmap_head, -90);
        else if(snake.front()->GetVelocity() == QPoint(-16,0))
            snake.front()->Draw(&painter, m_pixmap_head, 0);
        else if(snake.front()->GetVelocity() == QPoint(16,0))
            snake.front()->Draw(&painter, m_pixmap_head, 180);

        // Рисуем тело змеи
        for (int i = snake.size()-1; i > 0; --i)
        {
            if(snake[i]->GetVelocity() == QPoint(-16,0))
            {
                if(m_tl)
                    DrawToTurn(&painter, i, 90,-90);
                else if(m_bl)
                    DrawToTurn(&painter, i,90,0);
                else
                    DrawToDir(&painter, i,90,0);

                m_br = true;
                m_tr = true;
            }
            else if(snake[i]->GetVelocity() == QPoint(16,0))
            {
                if(m_tr)
                    DrawToTurn(&painter, i, -90, 180);
                else if(m_br)
                    DrawToTurn(&painter, i, -90, 90);
                else
                    DrawToDir(&painter, i,-90,0);

                m_bl = true;
                m_tl = true;
            }
            else if(snake[i]->GetVelocity() == QPoint(0,16))
            {
                if(m_tr)
                    DrawToTurn(&painter, i,0, -180);
                else if(m_tl)
                    DrawToTurn(&painter, i, 0,-90);
                else
                    DrawToDir(&painter, i,0,90);

                m_bl = true;
                m_br = true;
            }
            else if(snake[i]->GetVelocity() == QPoint(0,-16))
            {
                if(m_br)
                    DrawToTurn(&painter, i,180,90);
                else if(m_bl)
                    DrawToTurn(&painter, i,180,0);
                else
                    DrawToDir(&painter, i,180,90);

                m_tl = true;
                m_tr = true;
            }
        }
    }
}

void Widget::DrawTextPause(QPainter &p)
{
    QFont font;
    font.setPixelSize(40);
    p.setFont(font);
    int width_text = 350;
    int height_text = 100;
    QRect rect;
    rect.setX(m_width_window/2-width_text/2);
    rect.setY(m_height_window/2-height_text/2);
    rect.setWidth(width_text);
    rect.setHeight(height_text);
    p.drawText(rect, Qt::AlignCenter,"P A U S E\npress space to play");
}

void Widget::DrawScore(QPainter& p)
{
    QFont font;
    font.setPixelSize(16);
    font.setBold(true);
    p.setFont(font);
    p.drawText(16*2,0,250,16,Qt::TextSingleLine,"S C O R E: " + QString::number(score_));
}
