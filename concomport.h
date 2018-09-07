#ifndef CONCOMPORT_H
#define CONCOMPORT_H
#include <QObject>
#include <QLineEdit>
#include <QLabel>
#include <QTime>
#include "dataformat.h"


class concomport
{
public:
    //建構子
    concomport(QString com, QString Baudrate);
    //openSerialPort 開啟Port
    bool openSerialPort();
    //CheckStatus 檢查連接的port設備是否有選擇的port
    bool checkSerialStatus();
    //關閉port
    void closeSerialPort();
    //持續接收資料
    void recepackage();
    //檢查資料
    bool checkDLEData(QByteArray checkdata,int length);
    //初始化變數
    void init();
    //調用運算的cpp並運算
    void computeDLEdata();
    //設定畫面文字
    bool setText();
    void checkdataStatus();
public:
    QLineEdit *lineEdit_1;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QLineEdit *lineEdit_7;
    QLineEdit *lineEdit_8;
    QLineEdit *lineEdit_9;
    QLineEdit *lineEdit_10;
    QLineEdit *lineEdit_11;
    QLabel  *labalmessage;
    bool checkhavedata=false;
    dataformat un;




private:

    //array length
    int ResulfHEX_length;
    int RxHEX_length;
    int RxNODLE_length;
    int RxHEX_temp_length;



    //cwt,twt
    QByteArray CWT;
    QByteArray TWT;

    //array
    QByteArray ResulfHEX;
    QByteArray AHEX;
    QByteArray BHEX;
    //tx
    QByteArray TxHEX;


    QByteArray RxHEX;
    QByteArray temp_Rx;
    QByteArray RxNODLE;

    QString com;
    QString Baudrate;
    QString Operater;

    QString AUlltoStr;
    QString BUlltoStr;
    QString resultUlltoStr;

    bool checkTWTStatus;
    bool checkCWTStatus;
    bool checkisHaveData;
    bool checkDataiferror;

    bool checkSTX;
    bool checkETX;
    bool checkLRC;
    bool checkDLE;
    bool checkLength;
    int  checkDLEError;

    double CWT_test;
    double temp_CWT_test;
    double temp_CWT_test_2;
    double TWT_test;
    double timeout_test;


    //time
    QTime TWT_time;




};






#endif // CONCOMPORT_H
