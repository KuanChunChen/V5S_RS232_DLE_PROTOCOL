#ifndef dlecomputing_H
#define dlecomputing_H
#include <QObject>


class dlecomputing
{
public:
    dlecomputing(QByteArray RxNODLE,int RXNODLE_size);

    unsigned long long AddFunction(unsigned long long uInt_A, unsigned long long uInt_B);
    unsigned long long SubFunction(unsigned long long uInt_A,unsigned long long uInt_B);
    unsigned int MulFunction(unsigned int uInt_A,unsigned int uInt_B);
    unsigned int DivFunction(unsigned long long uInt_A,	unsigned long long uInt_B);
    unsigned int ORFunction(unsigned int uInt_A,unsigned int uInt_B);
    unsigned int XORFunction(unsigned long long uInt_A, unsigned long long uInt_B);
    void setAandBHEX(QByteArray RxNODLE, int halfInt, int RXNODLE_size);
    void intToByte(unsigned int uresultnumber);
    QByteArray getResultHEX();
    QByteArray getAHEX();
    QByteArray getBHEX();
    QString getOperater();
    QString getUllToStrA();
    QString getUllToStrB();
    QString getUllToStrResult();

private:

    unsigned long long uInt_A=0;
    unsigned long long  uInt_B=0;
    unsigned long long  resultdata;
    QByteArray AHEX;
    QByteArray BHEX;
    QByteArray ResultHEX;
    QString operater;


};

#endif // dlecomputing_H
