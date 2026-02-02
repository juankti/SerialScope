#include "serialhandler.h"
#include <QDebug>

serialhandler::serialhandler(ringbuffer* buffer, QObject* parent) :QObject(parent), m_buffer(buffer){
    connect(&m_serial, &QSerialPort::readyRead,this,&serialhandler::onReadyRead);
}

serialhandler::~serialhandler(){
    if (m_serial.isOpen()) m_serial.close();
}

void serialhandler::connectPort(QString portname,int baudrate){
    if (m_serial.isOpen()) m_serial.close();

    m_serial.setPortName(portname);
    m_serial.setBaudRate(baudrate);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);

    if(m_serial.open(QIODevice::ReadOnly)){
        m_buffer->clear();
        qDebug()<<" connected to "<<portname;
    }
}

void serialhandler::disconnectPort(){
    m_serial.close();
}

void serialhandler::onReadyRead(){
    QByteArray data = m_serial.readAll();

    for (char byte: data){
        unsigned char val = static_cast<unsigned char>(byte);
        double voltage =( val/255.0)*5;

        if (m_buffer) m_buffer->push(voltage);
    }
}
