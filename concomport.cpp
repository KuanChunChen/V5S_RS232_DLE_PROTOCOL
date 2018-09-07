#include "concomport.h"
#include <QSerialPort>
#include <QObject>
#include <QDebug>
#include <QSerialPortInfo>
#include <stdio.h>
#include "dataformat.h"
#include <QString>
#include "dlecomputing.h"
#include "mainwindow.h"
#include <QTime>



QSerialPort serial;




concomport::concomport(QString com , QString Baudrate)
{
    //取得傳入的com ,baudrate
    this->com=com;
    this->Baudrate=Baudrate;
    qDebug()<<this->com<<" "<<this->Baudrate;
    serial.clear();

    //初始化變數
    init();
}



bool concomport::openSerialPort()
{
   qDebug() <<"-------step1.1：openComport-------";



    if(com=="COM3" && Baudrate=="115200" && checkSerialStatus())
    {   serial.setPortName(com);
        serial.setBaudRate(QSerialPort::Baud115200);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);
        serial.clear();
        serial.open(QIODevice::ReadWrite);
        qDebug() <<"Aleady Open!!!";
        qDebug() <<"-------step1.2：recevie now-------";
        return true;
    }else
    {
        lineEdit_7->setText("serialportNotConnect");
        checkTWTStatus=false;
        checkCWTStatus=false;
        return false;
    }
}







void concomport::recepackage(){
    RxHEX.clear();
    qDebug()<<"wait time";
    QTime CWT_time;

    while(checkisHaveData)
    {   //temp空array的時候開始計時
        //代表開始接收
        //如果超過5s就結束
        //ctw超過1s就結束


        if(RxHEX.isEmpty())
        {
            TWT_time.start();
            //空array記錄當下時間點
            //非空array時記錄到的點代表收到第一個值的點

        }




        if((serial.bytesAvailable()>0||serial.waitForReadyRead(10) ) && TWT_test<5000)
        {   //有資料就收
            temp_Rx = serial.readAll();

            //完整陣列
            RxHEX+=temp_Rx;



            //記錄當下的長度
            RxHEX_length = RxHEX.length();

            //test::
            //紀錄上一段收到的時間點
            //temp_CWT_test=temp_CWT_test_2;
            //記錄收到後的時間點
            //temp_CWT_test_2=CTW_time.elapsed();
            //算出兩個字元間的時間
            //CWT_test=CTW_time.elapsed();
            //CWT_test=temp_CWT_test_2-temp_CWT_test;

            //如果收到tailer 就跳出迴圈 ,並清除後面資料
            if(RxHEX_length>2 &&RxHEX[RxHEX_length-2]==0x10 &&RxHEX[RxHEX_length-1]==0x03)
            {   qDebug()<<RxHEX.toHex();
                serial.clear();
                checkdataStatus();

                return;
            }

            //下次再收到資料從新計時
            //不進這個判斷式的話時間會繼下去
            CWT_time.start();
        }



        CWT_test=CWT_time.elapsed();
        //記錄整段收的時間
        TWT_test=TWT_time.elapsed();



        //看時間
        //qDebug()<<"TWT:"<<TWT_test;
        //qDebug()<<"CWT:"<<CWT_test;
        //if(timeout_test>1000)
        //qDebug() <<"------";

        if(TWT_test>5000)
        {
            checkisHaveData=false;
            //進行packing
            TWT=un.packingnow(TWT,2);
            qDebug() <<"TWT:"<<TWT_test;

            serial.write(TWT);

            checkTWTStatus=false;

            //超過五秒後如果還有送出值 直接清除
            while(serial.waitForReadyRead(1000)){
                if(serial.bytesAvailable()>0||serial.waitForReadyRead(10) )
                {
                    serial.clear();

                }
            }


            return;
        }else if(CWT_test>1000)
        {
            checkisHaveData=false;

            //進行packing
            CWT=un.packingnow(CWT,2);

            qDebug() <<"CWT:"<<CWT_test;
            serial.write(CWT);


            checkCWTStatus=false;

            return;
        }




    }


}




void concomport::checkdataStatus()
{
    if(!RxHEX.isEmpty() )
   {   //check DLE是否正確
       //回傳int 不同值代表不同狀態
       //qDebug()<<RxHEX.toHex();
       checkDLEError=un.checkDLEData(RxHEX,RxHEX_length);



       if(checkDLEError==5)
       {    //資料正確

            RxNODLE=un.getRxNODLE();
            RxNODLE_length=RxNODLE.length();
            checkisHaveData=false;
            checkDataiferror=true;


       }else if(checkDLEError==4  && RxHEX.length()>1 )
       {    //LRC ERROR
            checkLRC=false;
            checkisHaveData=false;
            checkTWTStatus=false;
            checkCWTStatus=false;



       }else if(checkDLEError==3  &&RxHEX.length()>1)
       {    qDebug()<<"DLE error";
            //DLE 0x10 ERROR
            checkDLE=false;
            checkisHaveData=false;
            checkTWTStatus=false;
            checkCWTStatus=false;

       }else if(checkDLEError==2  &&RxHEX.length()>1)
       {    //ETX ERROR
            checkETX=false;
            checkisHaveData=false;
            checkTWTStatus=false;
            checkCWTStatus=false;
       }else if(checkDLEError==1  &&RxHEX.length()>1)
       {    //STX ERROR
            checkSTX=false;
            checkisHaveData=false;
            checkTWTStatus=false;
            checkCWTStatus=false;

       }else if(checkDLEError==0  &&RxHEX.length()>1)
       {    //length ERROR
            checkLength=false;
            checkisHaveData=false;
            checkTWTStatus=false;
            checkCWTStatus=false;

       }else if(checkDLEError==6  &&RxHEX.length()>1)
       {
            //ETX ERROR
            checkLength=false;
            checkisHaveData=false;
            checkTWTStatus=false;
            checkCWTStatus=false;

       }
   }
}


void concomport::computeDLEdata(){
    //DLE protocol正確才運算
    if(checkTWTStatus &&checkCWTStatus &&checkDataiferror){
    dlecomputing computeDLE(RxNODLE,RxNODLE_length);
    AHEX = computeDLE.getAHEX();
    BHEX = computeDLE.getBHEX();
    ResulfHEX = computeDLE.getResultHEX();
    ResulfHEX_length=ResulfHEX.length();
    Operater = computeDLE.getOperater();
    AUlltoStr= computeDLE.getUllToStrA();
    BUlltoStr= computeDLE.getUllToStrB();
    resultUlltoStr = computeDLE.getUllToStrResult();

    //------- packing-------
    TxHEX=un.packingnow(ResulfHEX,ResulfHEX_length);
    serial.clear();

    serial.write(TxHEX);
    //serial.clear();

    }




}

bool concomport::setText()
{   //設定各種狀態的顯示方式
    if(checkCWTStatus  && checkTWTStatus){
    lineEdit_1->setText(RxHEX.toHex());
    lineEdit_2->setText(RxNODLE.toHex());
    lineEdit_3->setText(AHEX.toHex());
    lineEdit_4->setText(AUlltoStr);
    lineEdit_5->setText(ResulfHEX.toHex());
    lineEdit_6->setText(TxHEX.toHex());
    lineEdit_7->setText("sueessful");
    lineEdit_8->setText(Operater);
    lineEdit_9->setText(BHEX.toHex());
    lineEdit_10->setText(BUlltoStr);
    lineEdit_11->setText(resultUlltoStr);
    return true;
    }else if(!checkTWTStatus &&checkCWTStatus)
    {
        lineEdit_1->setText("");
        lineEdit_2->setText("");
        lineEdit_3->setText("");
        lineEdit_4->setText("");
        lineEdit_5->setText("");
        lineEdit_6->setText(TWT.toHex());
        lineEdit_7->setText("TWT error");
        lineEdit_8->setText("");
        lineEdit_9->setText("");
        lineEdit_10->setText("");
        lineEdit_11->setText("");
    return true;
    }else if(checkTWTStatus && !checkCWTStatus)
    {   lineEdit_1->setText("");
        lineEdit_2->setText("");
        lineEdit_3->setText("");
        lineEdit_4->setText("");
        lineEdit_5->setText("");
        lineEdit_6->setText(CWT.toHex());
        lineEdit_7->setText("CWT error");
        lineEdit_8->setText("");
        lineEdit_9->setText("");
        lineEdit_10->setText("");
        lineEdit_11->setText("");
    return true;
    }else if (checkLRC==false)
    {   lineEdit_1->setText(RxHEX.toHex());
        lineEdit_2->setText("");
        lineEdit_3->setText("");
        lineEdit_4->setText("");
        lineEdit_5->setText("");
        lineEdit_6->setText("");
        lineEdit_7->setText("LRC error");
        lineEdit_8->setText("");
        lineEdit_9->setText("");
        lineEdit_10->setText("");
        lineEdit_11->setText("");
    return true;
    }else if (checkSTX==false)
    {   lineEdit_1->setText("");
        lineEdit_2->setText("");
        lineEdit_3->setText("");
        lineEdit_4->setText("");
        lineEdit_5->setText("");
        lineEdit_6->setText("");
        lineEdit_7->setText("STX error");
        lineEdit_8->setText("");
        lineEdit_9->setText("");
        lineEdit_10->setText("");
        lineEdit_11->setText("");
    return true;
    }
    else if (checkETX==false)
    {   lineEdit_1->setText("");
        lineEdit_2->setText("");
        lineEdit_3->setText("");
        lineEdit_4->setText("");
        lineEdit_5->setText("");
        lineEdit_6->setText("");
        lineEdit_7->setText("ETX error");
        lineEdit_8->setText("");
        lineEdit_9->setText("");
        lineEdit_10->setText("");
        lineEdit_11->setText("");
    return true;
    }else if (checkDLE==false)
    {   lineEdit_1->setText(RxHEX.toHex());
        lineEdit_2->setText("");
        lineEdit_3->setText("");
        lineEdit_4->setText("");
        lineEdit_5->setText("");
        lineEdit_6->setText("");
        lineEdit_7->setText("DLE error");
        lineEdit_8->setText("");
        lineEdit_9->setText("");
        lineEdit_10->setText("");
        lineEdit_11->setText("");
    return true;
    }
    else if (checkLength==false)
        {   lineEdit_1->setText(RxHEX.toHex());
            lineEdit_2->setText("");
            lineEdit_3->setText("");
            lineEdit_4->setText("");
            lineEdit_5->setText("");
            lineEdit_6->setText("");
            lineEdit_7->setText("Resend error");
            lineEdit_8->setText("");
            lineEdit_9->setText("");
            lineEdit_10->setText("");
            lineEdit_11->setText("");
        return true;
        }




}



bool concomport::checkSerialStatus()
{
    QString returnport;
    //取得rs232資訊,test用
    qDebug() <<"-------step0：checkinfo-------";
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {

//            qDebug() << "Name        : " << info.portName();
//            qDebug() << "Description : " << info.description();
//            qDebug() << "Manufacturer: " << info.manufacturer();
//            qDebug() << "Location    : " << info.systemLocation();
//            qDebug() << "Busy        : " << info.isBusy();
//            qDebug() << "productId   : " << info.productIdentifier();
//            qDebug() << "serialNum   : " << info.serialNumber();
            //qDebug() << "bytesAvaila : " << info.bytesAvailable();
            //qDebug() << "bytesToWrit : " << info.bytesToWrite();
            //qDebug() << "Vendor Identifier: " << info.systemLocation();
            //如果線上有指定的poet,return true
            if(com==info.portName())
                return true;
            else{
                return false;
                qDebug() << "com device not found.";
            }


    }

}

void concomport::closeSerialPort()
{

    qDebug() <<"-------stopProgram-------";
    if(serial.isOpen())
    {
        serial.close();
        qDebug()<< "close `now!";
    }
    else{
        qDebug()<< "already close!";
    }

}






void concomport::init()
{
    //Qbytearray
    ResulfHEX.clear();
    TxHEX.clear();
    CWT.clear();
    TWT.clear();
    RxNODLE.clear();
    RxHEX.clear();
    temp_Rx.clear();


    AHEX.clear();
    BHEX.clear();
    ResulfHEX.clear();
    Operater.clear();
    AUlltoStr.clear();
    BUlltoStr.clear();
    resultUlltoStr.clear();



    //CWT
    CWT[0]=0xff;
    CWT[1]=0xff;

    //TWT
    TWT[0]=0xff;
    TWT[1]=0xfe;


    checkTWTStatus=true;
    checkCWTStatus=true;
    checkisHaveData=true;
    checkDataiferror=false;
    checkDLEError=7;
    //checkDLEProtocol
    checkSTX=true;
    checkETX=true;
    checkLRC=true;
    checkLength=true;
    checkDLE=true;

    CWT_test=0;
    temp_CWT_test=0;
    temp_CWT_test_2=0;
    TWT_test=0;
    ResulfHEX_length=0;

}


