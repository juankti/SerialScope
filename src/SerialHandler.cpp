#include "serialhandler.h"
#include <QDebug>

serialhandler::serialhandler(rawringbuffer* bufferCH1, rawringbuffer* bufferCH2, QObject* parent) 
    : QObject(parent), m_bufferCH1(bufferCH1), m_bufferCH2(bufferCH2){
    connect(&m_serial, &QSerialPort::readyRead,this,&serialhandler::onReadyRead);
}

serialhandler::~serialhandler(){
    if (m_serial.isOpen()) m_serial.close();
}

void serialhandler::connectPort(QString portname,int baudrate){
    if (m_serial.isOpen()) m_serial.close();

    m_serial.setPortName(portname);
    m_serial.setBaudRate(baudrate);

    // default configs
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);

    if(m_serial.open(QIODevice::ReadWrite)){
        m_serial.setDataTerminalReady(true);
        if (m_bufferCH1) m_bufferCH1->clear();
        if (m_bufferCH2) m_bufferCH2->clear();
        m_ch1Next = true;
        qDebug()<<" connected to "<<portname;
    }
}

void serialhandler::disconnectPort(){
    m_serial.close();
}

void serialhandler::onReadyRead(){
    QByteArray data = m_serial.readAll();

    for (char byte: data){
        uint8_t val = static_cast<uint8_t>(byte);

        if (m_dualChannel) {
            if (m_ch1Next) {
                if (m_bufferCH1) m_bufferCH1->push(val);
            } else {
                if (m_bufferCH2) m_bufferCH2->push(val);
            }
            m_ch1Next = !m_ch1Next;
        } else {
            if (m_bufferCH1) m_bufferCH1->push(val);
        }
    }
}
