#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QObject>
#include <QSerialPort>
#include "ringbuffer.h"

class serialhandler: public QObject{
    Q_OBJECT
public:
    explicit serialhandler(ringbuffer* buffer, QObject* parent =nullptr);
    ~serialhandler();

    QSerialPort* rawPort() {return &m_serial;}

public slots:
    void connectPort(QString portname, int baudrate);
    void disconnectPort();

private slots:
    void onReadyRead();

private:
    QSerialPort m_serial;
    ringbuffer* m_buffer;
};


#endif // SERIALHANDLER_H
