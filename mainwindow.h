#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <QLineEdit>
#include <QObject>
#include "concomport.h"
#include <QMainWindow>
#include "changeui.h"
#include <QThread>


//名稱空間 namespace
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    concomport * p_changeUI;
    bool buttinclick =true;




    \


private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();




private:
    Ui::MainWindow *ui;
private:

    QString  com;
    QString  baudrate;
    bool DLEstatus=false;

};









#endif // MAINWINDOW_H
