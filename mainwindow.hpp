#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include <QMainWindow>
#include <ui_mainwindow.h>
#include <memory>
namespace Ui {
class MainWindow;
}
struct impl;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    impl * p_impl;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow( );

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_clicked();
    void keyPressEvent(QKeyEvent *);
    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
