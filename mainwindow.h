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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_agree_clicked();
    void on_pingButton_clicked(QString ip);
    void on_a_click();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
