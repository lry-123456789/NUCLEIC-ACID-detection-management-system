#include "mainwindow_tetris.h"
#include "ui_mainwindow_tetris.h"
#include <qwidget.h>
/*
MainWindow_tetris::MainWindow_tetris(QWidget *parent) :
    QMainWindow(parent),
    ui1(new Ui::MainWindow_tetris)
{
    ui1->setupUi(this);
}

MainWindow_tetris::~MainWindow_tetris()
{
    delete ui1;
}
*/
MainWindow_tetris::MainWindow_tetris(QWidget *parent)
    : QMainWindow(parent)
{
    //创建对象
    tetrisBox = new TetrisBox;
    nextTetrisBox = new NextTetrisBox;
    nextTetrisLabel = new QLabel(tr("下一个："));
    controlLabel = new QLabel(tr("控制："));
    w_controlLabel = new QLabel(tr("W-旋转"));
    s_controlLabel = new QLabel(tr("S-向下移动"));
    a_controlLabel = new QLabel(tr("A-向左移动"));
    d_controlLabel = new QLabel(tr("D-向右移动"));
    h_controlLabel = new QLabel(tr("H-开始"));
    j_controlLabel = new QLabel(tr("J-暂停"));
    c_controlLabel = new QLabel(tr("C-重新开始"));
    m_controlLabel = new QLabel(tr("M-结束游戏"));
    scoreTitleLabel = new QLabel(tr("得分："));
    scoreLabel = new QLabel(tr("0"));
    mainLayout = new QGridLayout;
    //设置mainLayout的水平和横向的间隔为20
    mainLayout->setHorizontalSpacing(20);
    mainLayout->setVerticalSpacing(20);
    //设置mainLayout居中
    mainLayout->setAlignment(Qt::AlignCenter);
    //添加各个widget
    mainLayout->addWidget(tetrisBox, 0, 0, 14, 1);
    mainLayout->addWidget(nextTetrisLabel, 0, 1);
    mainLayout->addWidget(nextTetrisBox, 1, 1, 1, 2);
    mainLayout->addWidget(controlLabel, 5, 1);
    mainLayout->addWidget(w_controlLabel, 6, 1);
    mainLayout->addWidget(s_controlLabel, 6, 2);
    mainLayout->addWidget(a_controlLabel, 7, 1);
    mainLayout->addWidget(d_controlLabel, 7, 2);
    mainLayout->addWidget(h_controlLabel, 8, 1);
    mainLayout->addWidget(j_controlLabel, 8, 2);
    mainLayout->addWidget(c_controlLabel, 9, 1);
    mainLayout->addWidget(m_controlLabel, 9, 2);
    mainLayout->addWidget(scoreTitleLabel, 12, 1);
    mainLayout->addWidget(scoreLabel, 12, 2);

    //因为mainWindow已有一个layout，所以不能直接将mainLayout
    //设置到mainWindow中，需要先将mainLayout设置为一个widget的layout
    //在将widget设置为mainLayout的centralWidget
    QWidget *widget = new QWidget(this);
    widget->setLayout(mainLayout);
    setCentralWidget(widget);

    //设置窗口背景为灰色
    setPalette(Qt::gray);
    //设置窗口在电脑屏幕上居中
    /*
    QScreen *desktopWidget = QApplication::screens();
    int w = (desktopWidget->width() - this->width()) / 2;
    int h = 5;
    move(w, h);
    */

    //初始化
    status = STATUS_OFF;
    nextTetrisBox->updateNextTetris(tetris);
    setWindowTitle(tr("Game_Tetris - OFF"));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
}

MainWindow_tetris::~MainWindow_tetris()
{

}


//相应键盘事件
void MainWindow_tetris::keyPressEvent(QKeyEvent *event)
{
    //W键-进行旋转并更新游戏窗口内容
    if (event->key() == Qt::Key_W)
    {
        if (tetris.rotate())
        {
            //需要游戏状态为：正常进行
            if (status == STATUS_ON)
            {
                tetrisBox->updateTetris(tetris);
            }
        }
    }
    //A键-将方块向左移动并更新游戏窗口内容
    else if (event->key() == Qt::Key_A)
    {
        //需要游戏状态为：正常进行
        if (status == STATUS_ON)
        {
            if (tetris.moveToLeft())
            {
                tetrisBox->updateTetris(tetris);

            }
        }
    }
    //S键-将方块向下移动并更新游戏窗口内容
    else if (event->key() == Qt::Key_S)
    {
        //需要游戏状态：正常进行
        if (status == STATUS_ON)
        {
            if (tetris.moveToBottom())
            {
                tetrisBox->updateTetris(tetris);
                nextTetrisBox->updateNextTetris(tetris);
                updateScore();
            }
            else    //游戏结束
            {
                //计时器停止
                timer->stop();
                //输出结束提示
                QString str;
                str +=  QString("Game Over!\nYour Score is: %1!").arg(tetris.getScore());
                QMessageBox::information(this, tr("Game Over"), str);
                //更改游戏状态为：游戏结束
                status = STATUS_END;
                setWindowTitle(tr("Game_Tetris - END"));
            }
        }
    }
    //D键-将方块向右移动并更新游戏窗口内容
    else if (event->key() == Qt::Key_D)
    {
        //需要游戏状态为：正常进行
        if (status == STATUS_ON)
        {
            if (tetris.moveToRight())
            {
                tetrisBox->updateTetris(tetris);
            }
        }
    }
    //H键-开始游戏
    //不同状态的相应：
    //之前状态    之后状态
    //游戏暂停 -> 正常进行
    //还未开始 -> 正常进行
    //游戏结束 -> 正常进行
    else if (event->key() == Qt::Key_H)
    {
        if (status == STATUS_PAUSE)
        {
            timer->start(500);
            status = STATUS_ON;
            setWindowTitle(tr("Game_Tetris - ON"));
        }
        else if (status == STATUS_OFF)
        {
            //初始化窗口视图
            tetris.createBlock();
            tetrisBox->updateTetris(tetris);
            nextTetrisBox->updateNextTetris(tetris);
            updateScore();

            status = STATUS_ON;
            setWindowTitle(tr("Game_Tetris - ON"));
            timer->start(500);
        }
        else if (status == STATUS_END)
        {
            //初始化tetris
            tetris.clear();
            tetris.createBlock();
            tetrisBox->updateTetris(tetris);
            nextTetrisBox->updateNextTetris(tetris);
            updateScore();

            status = STATUS_ON;
            setWindowTitle(tr("Game_Tetris - ON"));
            timer->start(500);
        }
    }
    //J键-游戏暂停
    else if (event->key() == Qt::Key_J)
    {
        //需要游戏状态为：正常进行
        if (status == STATUS_ON)
        {
            timer->stop();
            status = STATUS_PAUSE;
            setWindowTitle(tr("Game_Tetris - PAUSE"));
        }
    }
    //C键-重新开始游戏
    else if (event->key() == Qt::Key_C)
    {
        timer->stop();
        tetris.clear();
        tetrisBox->updateTetris(tetris);
        nextTetrisBox->updateNextTetris(tetris);
        updateScore();
        status = STATUS_OFF;
        setWindowTitle(tr("Game_Tetris - OFF"));

    }
    //M键-关闭游戏
    else if (event->key() == Qt::Key_M)
    {
        close();
    }

}


void MainWindow_tetris::onTimer()
{
    if(tetris.moveToBottom())
    {
        tetrisBox->updateTetris(tetris);
        nextTetrisBox->updateNextTetris(tetris);
        updateScore();
    }
    else
    {
        timer->stop();
        QString str;
        str +=  QString("Game Over!\nYour Score is: %1!").arg(tetris.getScore());
        QMessageBox::information(this, tr("Game Over"), str);
        status = STATUS_END;
        setWindowTitle(tr("Game_Tetris - END"));
    }
}


void MainWindow_tetris::updateScore()
{
    QString str;
    int score = tetris.getScore();
    str += QString("%1").arg(score);
    scoreLabel->setText(str);
}


//若窗口最小化就停止计时器
void MainWindow_tetris::changeEvent(QEvent *event)
{
    if (event->type() != QEvent::WindowStateChange)
    {
        return;
    }
    if (windowState() == Qt::WindowMinimized)
    {
        timer->stop();
    }
}
