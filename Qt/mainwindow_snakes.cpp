#include "mainwindow_snakes.h"
#include "ui_mainwindow_snakes.h"

MainWindow_snakes::MainWindow_snakes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_snakes)
{
    ui->setupUi(this);
}

MainWindow_snakes::~MainWindow_snakes()
{
    delete ui;
}
