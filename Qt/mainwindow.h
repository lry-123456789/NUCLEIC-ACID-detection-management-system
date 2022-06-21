#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mainwindow_snakes.h>
#include <mainwindow_tetris.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_lineEdit_4_textChanged(const QString &arg1);

    void on_lineEdit_5_textChanged(const QString &arg1);

    void on_lineEdit_6_textChanged(const QString &arg1);

    void on_lineEdit_7_textChanged(const QString &arg1);

    void on_lineEdit_8_textChanged(const QString &arg1);

    void on_lineEdit_9_textChanged(const QString &arg1);

    void on_lineEdit_10_textChanged(const QString &arg1);

    void on_lineEdit_11_textChanged(const QString &arg1);

    void on_lineEdit_12_textChanged(const QString &arg1);

    void on_lineEdit_13_textChanged(const QString &arg1);

    void on_lineEdit_14_textChanged(const QString &arg1);

    void on_progressBar_valueChanged(int value);

    void on_lineEdit_15_textChanged(const QString &arg1);

    void on_lineEdit_9_textEdited(const QString &arg1);

    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_2_editingFinished();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

private:
    Ui::MainWindow *ui;
private:
    Ui::MainWindow_snakes *ui0;
private:
    Ui::MainWindow_tetris *ui1;
private:
    QVector<QString> name;
    QVector<QString> gender;
    QVector<QString> detect_time;
    QVector<QString> health_code;
    QVector<QString> middle_high_risk_area;
    QVector<QString> last_detect_result;
    QVector<QString> is_quarantined;
    QVector<QString> vaccine_time;
    QVector<QString> vaccine_first_time;
    QVector<QString> vaccine_second_time;
    QVector<QString> vaccine_third_time;
    QVector<QString> vaccine_fourth_time;
};
#endif // MAINWINDOW_H
