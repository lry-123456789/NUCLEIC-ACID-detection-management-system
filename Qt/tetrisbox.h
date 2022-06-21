#ifndef TETRISBOX_H
#define TETRISBOX_H


#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QBrush>
#include <QColor>
//为了使用Block
#include "tetris.h"

class TetrisBox : public QWidget
{
    Q_OBJECT
public:
    explicit TetrisBox(QWidget *parent = nullptr);
    void updateTetris(Tetris tetris);       //更新数据和视图
    void paintEvent(QPaintEvent *event);    //绘制视图

signals:

public slots:

private:
    Block block;            //用来储存Tetris中block的数据
    int box[MAXX][MAXY];    //用来存储Tetris中box的数据
};


#endif // TETRISBOX_H
