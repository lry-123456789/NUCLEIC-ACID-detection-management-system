#ifndef MAINWINDOW_SNAKES_H
#define MAINWINDOW_SNAKES_H

#include <QMainWindow>

namespace Ui {
class MainWindow_snakes;
}

class MainWindow_snakes : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_snakes(QWidget *parent = nullptr);
    ~MainWindow_snakes();

private:
    Ui::MainWindow_snakes *ui;
};

#endif // MAINWINDOW_SNAKES_H
