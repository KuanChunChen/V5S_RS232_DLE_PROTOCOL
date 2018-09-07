#ifndef unpacking_H
#define unpacking_H
#include <QObject>


class dataformat
{
public:
    dataformat();

    //根據DLE拆封包
    void unPackingnow();
    void RemoveETX ();
    void RemoveSTX ();
    void RemoveAddDLE ();
    void RemoveLRC ();
    //檢查LRC是否正確
    bool checkRxLRC();
    //取得rxNoDLE
    QByteArray getRxNODLE();
    int getRxSize();
    int checkDLEData(QByteArray checkdata,int length);

    //packing
    packing(QByteArray ResultHex,int ResultHEX_size);
    void AddLRC();
    void ReaptDLE ();
    void AddSTX ();
    void AddETX ();
    QByteArray getTxdata();
    int getTxsize();
    QByteArray packingnow(QByteArray ByteHex, int ByteHEXsize);



private:
    //unpacking
    int RxSize;
    QByteArray Rx;
    QByteArray RxNODLE;
    //packing
    int TxSize;
    QByteArray Tx;
    //ox10
    int count;
    int index;
    //ckeck resend
    QByteArray Resend_buff;
    QByteArray Resend_buff_2;


};

#endif // unpacking_H
