#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_listWidget_clicked();

    void on_listWidget_2_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void timerUpdate();

    void on_pushButton_3_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
