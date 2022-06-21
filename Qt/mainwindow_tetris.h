#ifndef MAINWINDOW_TETRIS_H
#define MAINWINDOW_TETRIS_H

#include <QMainWindow>
/*
namespace Ui {
class MainWindow_tetris;
}

class MainWindow_tetris : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_tetris(QWidget *parent = nullptr);
    ~MainWindow_tetris();

private:
    Ui::MainWindow_tetris *ui;
};
*/

#include <QMainWindow>
#include <QMainWindow>
#include <QPainter>
#include <QEvent>
#include <QPaintEvent>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QKeyEvent>
#include <QTimer>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QScreen>
#include <QApplication>
#include "tetris.h"
#include "tetrisbox.h"
#include "nexttetrisbox.h"
namespace Ui { class MainWindow_tetris; }
QT_END_NAMESPACE

//游戏的状态
#define STATUS_ON 0     //游戏正常进行
#define STATUS_PAUSE 1  //游戏暂停
#define STATUS_OFF 2    //游戏未开始
#define STATUS_END 3    //游戏结束

class MainWindow_tetris : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_tetris(QWidget *parent = nullptr);
    ~MainWindow_tetris();
    void keyPressEvent(QKeyEvent *event);   //响应键盘事件
    void changeEvent(QEvent *event);        //窗口最小化后暂停
    void updateScore();                     //更新分数的数据和显示

public slots:
    void onTimer();

private:
    int status;                     //游戏状态
    Tetris tetris;                  //俄罗斯方块类对象
    QTimer *timer;                  //计时器
    TetrisBox *tetrisBox;           //游戏窗口
    NextTetrisBox *nextTetrisBox;   //“下一个”窗口
    QGridLayout *mainLayout;        //mainLayout
    QLabel *nextTetrisLabel;        //“下一个”窗口的标签
    QLabel *controlLabel;           //“控制”标签
    QLabel *w_controlLabel;         //W键的标签
    QLabel *s_controlLabel;         //S键的标签
    QLabel *a_controlLabel;         //A键的标签
    QLabel *d_controlLabel;         //D键的标签
    QLabel *h_controlLabel;         //H键的标签
    QLabel *j_controlLabel;         //J键的标签
    QLabel *c_controlLabel;         //C键的标签
    QLabel *m_controlLabel;         //M键的标签
    QLabel *scoreTitleLabel;        //分数标题标签
    QLabel *scoreLabel;             //分数标签（用来显示分数）
    Ui::MainWindow_tetris *ui1;
};

#endif // MAINWINDOW_TETRIS_H
