#include "dlecomputing.h"
#include <QDebug>
#include <QObject>

dlecomputing::dlecomputing(QByteArray RxNODLE,int RXNODLE_size)
{

    if(RXNODLE_size<2){
        return;
    }

    //取得陣列中位數
    int halfInt=RXNODLE_size/2;
    if(halfInt%2!=0)
    {
        halfInt-=1;
    }

    //設定a & b HEX
    setAandBHEX(RxNODLE , halfInt ,RXNODLE_size);
    qDebug()<<(AHEX+BHEX).toHex();



    //將資料存成int 方便運算
    qDebug()<<"-------AHEXcount-------";
    qDebug()<<"data:"<<AHEX.toHex()<<" "<<BHEX.toHex();
    for(int x=0;x<halfInt;x++){
        //利用移位改變數字大小x==halfInt-1時,不移位

        uInt_A = (uInt_A << 8) | static_cast<unsigned char>(AHEX[x]);
        qDebug()<<"A["<<x<<"]"<<AHEX.toHex()<<uInt_A;
    }
    qDebug()<<"-------BHEXcount-------";
    for(int y=0;y<halfInt;y++){

        //利用移位將數字存入
        uInt_B = (uInt_B << 8) | static_cast<unsigned char>(BHEX[y]);
        qDebug()<<"A["<<y<<"]"<<BHEX.toHex()<<uInt_B;


    }





    if(RxNODLE[1]==0x01)
    {
        operater="+";
        resultdata=AddFunction(uInt_A,uInt_B);
    }
    else if(RxNODLE[1]==0x02)
    {   operater="-";
        resultdata=SubFunction(uInt_A,uInt_B);
    }
    else if(RxNODLE[1]==0x04)
    {   operater="x";
        resultdata=MulFunction(uInt_A,uInt_B);
    }
    else if(RxNODLE[1]==0x08)
    {   operater="/";
        resultdata=DivFunction(uInt_A,uInt_B);

    }
    else if(RxNODLE[1]==0x10)
    {   operater="|";
        resultdata=ORFunction(uInt_A,uInt_B);
    }
    else if(RxNODLE[1]==0x20)
    {   operater="^";
        resultdata=XORFunction(uInt_A,uInt_B);
    }
    else
    {
        qDebug()<<"dlecomputing error!";
    }

    qDebug()<<"ucharAHEXToInt:"<< uInt_A<<"ucharBHEXToInt:"<< uInt_B<<"resultdata:"<<resultdata;


    //將運算玩的Int轉回qbytearray
    intToByte(resultdata);


}

unsigned long long dlecomputing::AddFunction(unsigned long long uInt_A, unsigned long long uInt_B)
{


    //兩個暫存int
    //進行邏輯閘運算
    //當uInt_B為0時回傳結果
    unsigned long long temp_sum=0,buf=0;
    if(uInt_B==0)
      return uInt_A;
    temp_sum = uInt_A^uInt_B;
    buf =(uInt_A&uInt_B)<<1;
    return AddFunction(temp_sum,buf);

}

unsigned long long dlecomputing::SubFunction(unsigned long long uInt_A, unsigned long long uInt_B)
{
    //減法實現
    while (uInt_B != 0)
        {
            unsigned long long sameBits = uInt_A & uInt_B;
            uInt_A ^= sameBits;
            uInt_B ^= sameBits;
            uInt_A |= uInt_B;
            uInt_B <<= 1;
        }

    return uInt_A;
}

unsigned int dlecomputing::MulFunction(unsigned int uInt_A, unsigned int uInt_B)
{
    //將乘數與被乘數取絕對值
    unsigned long long multiplier = uInt_A < 0 ?  AddFunction(~uInt_A , 1) : uInt_A;
    unsigned long long multiplicand = uInt_B < 0 ? AddFunction(~uInt_B, 1) : uInt_B;

    //計算絕對值得乘積
      int product = 0;
      while(multiplier)
        {
          if(multiplier & 0x1)
          {
            product = AddFunction(product, multiplicand);
          }

          multiplicand = multiplicand << 1;
          multiplier = multiplier >> 1;
        }

        //計算乘機的符號
        if((uInt_A ^ uInt_B) < 0)
        {
          product = AddFunction(~product, 1);
        }

        return product;
}

unsigned int dlecomputing::DivFunction(unsigned long long uInt_A, unsigned long long uInt_B)
{
    //被除數與除數取絕對值
      unsigned long long dividend = uInt_A < 0 ? AddFunction(~uInt_A, 1) : uInt_A;
      unsigned long long divisor = uInt_B < 0 ? AddFunction(~uInt_B, 1) : uInt_B;


      unsigned long long  invert = 2;
      while(dividend)
      {
        invert |= dividend & 0x1;
        invert = invert << 1;
        dividend = dividend >> 1;
      }

      unsigned long long quotient = 0;
      unsigned long long
              remainder = 0;
      while(invert > 1)//排除最高位的1
      {
        remainder = remainder << 1;
        remainder |= invert & 0x1;
        invert = invert >> 1;
        quotient = quotient << 1;

        if(remainder >= divisor)
        {
          quotient |= 0x1;
          remainder = SubFunction(remainder, divisor);
        }
      }

      //求商的符號
      if((uInt_A ^ uInt_B) < 0)
      {
        quotient = AddFunction(~quotient, 1);
      }

      return quotient;





}

//OR運算
unsigned int dlecomputing::ORFunction(unsigned int uInt_A, unsigned int uInt_B)
{
    unsigned int OR_buff;
    OR_buff=uInt_A|uInt_B;

    return OR_buff;

}
//XOR運算
unsigned int dlecomputing::XORFunction(unsigned long long uInt_A, unsigned long long uInt_B)
{
    unsigned long long XOR_buff;
    XOR_buff = uInt_A ^ uInt_B;

    return XOR_buff;

}

void dlecomputing::setAandBHEX(QByteArray RxNODLE ,int halfInt,int RXNODLE_size)
{
    qDebug()<<"-------AHEX-------";
    AHEX.clear();
    for(int b=2;b<halfInt+2;b++){
        AHEX[b-2]=(unsigned char)RxNODLE[b];
        qDebug()<<AHEX<<AHEX.length();

    }
    qDebug()<<"-------BHEX-------";
    BHEX.clear();
    for(int c=halfInt+2;c<RXNODLE_size;c++){
        BHEX[c-halfInt-2]=RxNODLE[c];
        qDebug()<<BHEX<<BHEX.length();
    }


}


void  dlecomputing::intToByte(unsigned int uresultnumber)
{
    //將資料型態轉回uchar
    ResultHEX.clear();
    unsigned char a = (uresultnumber >> 24) & 0XFF;
    unsigned char b = (uresultnumber >> 16) & 0XFF;
    unsigned char c = (uresultnumber >> 8) & 0XFF;
    unsigned char d = uresultnumber & 0XFF;
    ResultHEX.append(a);
    ResultHEX.append(b);
    ResultHEX.append(c);
    ResultHEX.append(d);


    qDebug()<<"-------ResultHEX-------"<<ResultHEX.toHex();

}

QByteArray dlecomputing::getResultHEX()
{
    return ResultHEX;
}
QByteArray dlecomputing::getAHEX()
{
    return AHEX;
}
QByteArray dlecomputing::getBHEX()
{
    return BHEX;
}
QString dlecomputing::getOperater()
{
    return operater;
}
QString dlecomputing:: getUllToStrA()
{   QString uStr_A;
    uStr_A=QString::number(uInt_A,10);
    return uStr_A;
}
QString dlecomputing:: getUllToStrB()
{   QString uStr_B;
    uStr_B=QString::number(uInt_B,10);
    return uStr_B;
}
QString dlecomputing:: getUllToStrResult()
{   QString uStr_result;
    uStr_result=QString::number(resultdata,10);
    return uStr_result;
}






