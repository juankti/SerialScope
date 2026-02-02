#include "serialcom.h"

serialcom::serialcom(QObject *parent, QSerialPort*port)
    : QObject{parent}
{
    this->port=port;
    connect(this->port,&QSerialPort::readyRead,this,&serialcom::handleReadReady);

}

void serialcom::handleReadReady(){
    emit dataReceived(port->readAll());
}

void serialcom::handleError(QSerialPort::SerialPortError error){
    if (error!= QSerialPort::NoError) emit _error(port->errorString());
}
