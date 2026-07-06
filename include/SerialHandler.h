#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QObject>
#include <QSerialPort>
#include "rawringbuffer.h"

class serialhandler: public QObject{
    Q_OBJECT
public:
    explicit serialhandler(rawringbuffer* bufferCH1, rawringbuffer* bufferCH2, QObject* parent =nullptr);
    ~serialhandler();

    QSerialPort* rawPort() {return &m_serial;}
    void setVRef(double vref) { m_vref = vref; }
    void setDualChannel(bool dual) { m_dualChannel = dual; m_ch1Next = true; }

public slots:
    void connectPort(QString portname, int baudrate);
    void disconnectPort();

private slots:
    void onReadyRead();

private:
    QSerialPort m_serial;
    rawringbuffer* m_bufferCH1;
    rawringbuffer* m_bufferCH2;
    double m_vref = 5.0;
    bool m_dualChannel = false;
    bool m_ch1Next = true;
};


#endif // SERIALHANDLER_H
