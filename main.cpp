#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QObject>
//#include <QSerialPort>
//#include <QSerialPortInfo>
#include "concomport.h"


int main(int argc, char *argv[])
{

    //show UI
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    //這邊才顯示視窗
}
