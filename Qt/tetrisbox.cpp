#include "tetrisbox.h"

TetrisBox::TetrisBox(QWidget *parent) : QWidget(parent)
{
    //对block初始化
    for (int i = 0; i < COUNT; i++)
    {
        block.x[i] = -1;
        block.y[i] = -1;
    }
    block.centerX = -1;
    block.centerY = -1;
    block.ID = -1;
    //对box初始化
    for (int i = 0; i < MAXX; i++)
    {
        for (int j = 0; j < MAXY; j++)
        {
            box[i][j] = 0;
        }
    }

    //设置本游戏窗口的宽度和高度
    //并设置背景为黑色
    int w = Tetris::getWidth();
    int h = Tetris::getHeight();
    setFixedSize(w, h);
    setPalette(QPalette(Qt::black));
    setAutoFillBackground(true);
}


void TetrisBox::updateTetris(Tetris tetris)
{
    //更新block
    block = tetris.getBlock();
    //更新box
    for (int i = 0; i < MAXX; i++)
    {
        for (int j = 0; j < MAXY; j++)
        {
            box[i][j] = tetris.getBox(i, j);
        }
    }
    repaint();
}


void TetrisBox::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen;
    QBrush brush;
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(255, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(255, 255, 255));
    painter.setPen(pen);
    painter.setBrush(brush);

    //绘制box中的内容
    for (int i = 0; i < MAXX; i++)
    {
        for (int j = 0; j < MAXY; j++)
        {
            if (box[i][j] == 1)
            {
                int x = i * WIDTH + i * INTERVAL;
                int y = j * HEIGHT + j * INTERVAL;
                painter.drawRect(x, y, WIDTH, HEIGHT);
            }
        }
    }
    //绘制block中的内容
    for (int i = 0; i < COUNT; i++)
    {
        int x = block.x[i];
        int y = block.y[i];
        int x1 = x * WIDTH + x * INTERVAL;
        int y1 = y * HEIGHT + y * INTERVAL;
        painter.drawRect(x1, y1, WIDTH, HEIGHT);
    }
}
