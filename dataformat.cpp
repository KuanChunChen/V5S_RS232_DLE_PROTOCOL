#include "dataformat.h"
#include <QDebug>

dataformat::dataformat()
{
    Rx.clear();
    RxNODLE.clear();
    Tx.clear();
    RxSize=0;
    TxSize=0;
    count=0;
    index=0;
}

int dataformat::checkDLEData(QByteArray checkdata,int length){


    //判斷封包是否重送
    for(int j=2;j<length-1;j++)
    {
        if(checkdata[j]==16 && checkdata[j+1]==02)
        {
            index=j;
            qDebug()<<index;
            break;


            //紀錄下一次header再出現的位置
        };
    }
    for(int z=0;z<index;z++)
    {
        //紀錄header再次重複前的bytes
        Resend_buff[z]=checkdata[z];
    }

    for(int c=0;c<index;c++){
        //確認重複的byte個數
        if(Resend_buff[c]==checkdata[c+index])
        {
            count++;
        }
    }
    if(count>=index && index>1)
    {
        //判斷是否為resend
        return 6;
    }






    //確定陣列長度，避免error
    if(length<2)
    {
        return 0;
    }
    Rx=checkdata;
    RxSize=length;



    //check DLE header & tailer
    if(checkdata[0]!=16 &&checkdata[1]!=2){
        qDebug() <<"STX error";
        //ed_7->setText("STX error");


        return 1;

        if(checkdata[length-2]!=16 && checkdata[length-1]!=3)
        {
        qDebug() <<"ETX error2";
         //ed_7->setText("ETX error");
        return 2;

        }
    }
    //check 0x10 next element
    for(int i=2;i<length-3;i++){

        if(checkdata[i]==0x10){
            if(checkdata[i+1]==0x10)
            {
                qDebug() <<i+1<<"->"<<"0x10";

            }else if(checkdata[i+1]==2){

                qDebug() <<i+1<<"->"<<"0x02";
            }else if(checkdata[i+1]==3){
                qDebug() <<i+1<<"->"<<"0x03";
            }else if (checkdata[i+1]==8)
            {
                qDebug() <<i+1<<"->"<<"0x03";
            }else
            {
                qDebug() <<i+1<<"->"<<"DLE error"<<checkdata[i+1];

                 //ed_7->setText("DLE error");
                return 3;

            }
        }//end of if


    }//end of for





    //上面都正確後 將資料存入Rx,RxSize


    //先使用DLE建構子,在呼叫查看LRC是否正確


    unPackingnow ();
    if(!checkRxLRC())
    {
        qDebug() <<"LRC error3";
         //ed_7->setText("LRC error");
        return 4;


    }


    //change ed_2
    //ed_2->setText(QBA_buffer_2.toHex());

    qDebug() <<RxNODLE.toHex();
    return 5;
    //資料檢查正確



}

QByteArray dataformat::packingnow(QByteArray ByteHex, int ByteHEXsize)
{
    Tx.clear();
    TxSize=0;
    Tx=ByteHex;
    TxSize=ByteHEXsize;

    qDebug() <<"-------Tx:-------"<<Tx.toHex();

    AddLRC();
    ReaptDLE();
    AddSTX();
    AddETX();

    return Tx;
}




void dataformat::AddLRC()
{
    unsigned char LRC=0x00;

    for(int c=0;c<TxSize;c++){

        LRC^=Tx[c];
    }
    qDebug() <<"-------Tx_LRC:-------"<<LRC;
    Tx.append(LRC);
    TxSize=Tx.length();
    qDebug() <<"-------Tx_now:-------"<<Tx<<"Txsize:"<<Tx.length();


}

void dataformat::ReaptDLE()
{


    for(int g=0;g<TxSize;g++)
    {
        if(Tx[g]==0x10)
        {
         // 重複DLE 0x10
            for(int i=TxSize;i>g;i--)
            {
                Tx[i]=Tx[i-1];


            }
            //重複後 下一個index不再加0x10
            g=g+1;
            //txsize++
            TxSize=TxSize+1;

        }

    }
    qDebug() <<"-------Tx_DLE:-------"<<Tx<<"TxSize:"<<Tx.length();

}

void dataformat::AddSTX()
{

    for(int z=0;z<2;z++)
    {
        for(int w=TxSize;w>0;w--)
        {
            Tx[w]=Tx[w-1];
        }

        TxSize=TxSize+1;
    }

    //插入STX
    Tx[0]=0x10;
    Tx[1]=0x02;

    qDebug() <<"-------Tx_STX:-------"<<Tx<<"TxSize:"<<TxSize;


}

void dataformat::AddETX()
{
    Tx[TxSize]=0x10;
    TxSize++;
    Tx[TxSize]=0x03;
    TxSize++;
    qDebug() <<"-------Tx_ETX:-------"<<Tx<<"TxSize:"<<TxSize;


}

QByteArray dataformat::getTxdata()
{

    return Tx;

}


























void dataformat::unPackingnow()
{
    qDebug() <<"-------step2.0：start unpacking-------";
    if(RxSize<6)
    {
        return;
    }
    RemoveETX();
    RemoveSTX();
    RemoveAddDLE();
    RemoveLRC();
    //上面算完後把資料放入RxNODLE
    for(int g=0;g<RxSize;g++)
    {
        RxNODLE[g]=Rx[g];
    }




}



void dataformat::RemoveETX()
{   qDebug() <<"-------rmETX-------";
    if(RxSize<6){
        return;
    }
    RxSize = RxSize-2;


    qDebug()<<"RemoveETX:" <<Rx<<"Rxsize:"<<RxSize;
}

void dataformat::RemoveSTX()
{
    qDebug() <<"-------rmSTX-------";
    for(int w=0;w<2;w++){
        for(int k=0;k<RxSize;k++)
        {
            //i=0 ,i+1=1.
            //i=1 ,i+1=2.
            //i=2 ,i+1=3.
            Rx[k]=Rx[k+1];

        }
        //11
        RxSize=RxSize-1;
    }


        qDebug()<<"RemoveSTX:"<<Rx<<"RxSize"<<RxSize;


}

void dataformat::RemoveAddDLE()
{
    qDebug() <<"-------rmAddDLE-------";

    int index=0;

    for(int n=0;n<RxSize;n++){

        if(Rx[n]==0x10){
            //紀錄出現0x10的位置
            index=n;
            qDebug()<<"RAddDLEpostion: "<<n<<" "<<Rx[n];
            //輸出目前狀態

            //當出現0x10下一個也是時
            if(Rx[n+1]==0x10){
                for(int a=index+1;a<RxSize;a++){
                    Rx[a]=Rx[a+1];
                    //qDebug()<<"RAddDLEafter: "<<a<<" "<<Rx[a];

                }
                //刪除一個陣列大小
            RxSize=RxSize-1;
            }

        }

    }




        qDebug()<<"RemoveAddDLE:"<<Rx<<"Rxsize:"<<RxSize;





}

void dataformat::RemoveLRC()

{
    qDebug() <<"-------rmLRC-------";
    unsigned char LRC=0x00;
    for(int c=0;c<RxSize-1;c++){

        LRC^=Rx[c];
    }
    qDebug()<<"LRC:"<<LRC;
    //確認LRC
    if(Rx[RxSize-1]==LRC){
        qDebug()<<"LRC:"<<"correct";
        RxSize=RxSize-1;
        qDebug()<<"Rxsize"<<RxSize;

    }


}
//提供給concomport
bool dataformat::checkRxLRC()
{

    qDebug() <<"-------checkLRC-------";
    unsigned char LRC=0x00;
    for(int c=0;c<RxSize;c++){

        LRC^=Rx[c];
    }
    qDebug()<<"LRC:"<<LRC<<"conport:correct";
    //確認LRC
    if(Rx[RxSize]!=LRC)
    {
        qDebug()<<"LRC:"<<"false";

        return false;
    }
    return true;




}


QByteArray dataformat::getRxNODLE(){
    return RxNODLE;
}

int dataformat::getRxSize(){
    return RxSize;
}





