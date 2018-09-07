#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "concomport.h"

#include <QDebug>
#include <QObject>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);




}

MainWindow::~MainWindow()
{
 delete ui;
    QPushButton  *myBtn;
    myBtn = new QPushButton(this);
    QApplication::processEvents();

    connect(myBtn,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));



}

void MainWindow::on_pushButton_clicked()
{
    com = ui->comboBox->currentText();
    qDebug()<<com;
    baudrate = ui->comboBox_2->currentText();
    qDebug()<<baudrate;

    p_changeUI = new concomport(com,baudrate);
    p_changeUI->lineEdit_1 = ui->lineEdit;
    p_changeUI->lineEdit_2 = ui->lineEdit_2;
    p_changeUI->lineEdit_3 = ui->lineEdit_3;
    p_changeUI->lineEdit_4 = ui->lineEdit_4;
    p_changeUI->lineEdit_5 = ui->lineEdit_5;
    p_changeUI->lineEdit_6 = ui->lineEdit_6;
    p_changeUI->lineEdit_7 = ui->lineEdit_7;
    p_changeUI->lineEdit_8 = ui->lineEdit_8;
    p_changeUI->lineEdit_9 = ui->lineEdit_9;
    p_changeUI->lineEdit_10 = ui->lineEdit_10;
    p_changeUI->lineEdit_11 = ui->lineEdit_11;
    p_changeUI->labalmessage=ui->label_14;
    buttinclick=false;

QApplication::processEvents();
   if(p_changeUI->openSerialPort())
    {
    p_changeUI->recepackage();
//    p_changeUI->checkdataStatus();
    p_changeUI->computeDLEdata();

    }

    if(p_changeUI->setText())
    {
        QApplication::processEvents();
        delete p_changeUI;
        on_pushButton_clicked();


    }else
    {
        return;
    }
}








void MainWindow::on_pushButton_2_clicked()
{
    //stop_button_init
    QApplication::processEvents();
    com="";
    baudrate="";
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
    ui->lineEdit_5->setText("");
    ui->lineEdit_6->setText("");
    ui->lineEdit_7->setText("");
    ui->lineEdit_8->setText("");
    ui->lineEdit_9->setText("");
    ui->lineEdit_10->setText("");
    ui->lineEdit_11->setText("");
    ui->pushButton->setEnabled(true);

    p_changeUI->closeSerialPort();



}





