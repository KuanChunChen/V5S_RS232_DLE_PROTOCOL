#include "concoport.h"

concoport::concoport()
{
    serial = new QSerialPort(this);
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    openSerialPort();
}


void concoport::openSerialPort()
{
    serial->setPortName("COM3");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite)) {

        showStatusMessage("Connectedd");

    } else {

        showStatusMessage(tr("Open error"));
    }
}

void concoport::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();

    showStatusMessage(tr("Disconnected"));
}

void concoport::writeData(const QByteArray &data)
{
    serial->write(data);
}

void concoport::readData()
{
   QByteArray data = serial->readAll();

   qDebug() << data;

}

void concoport::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        closeSerialPort();
    }
}


void concoport::showStatusMessage(const QString &message)
{
    qDebug() << message;
}
