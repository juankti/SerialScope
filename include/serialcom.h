#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <QObject>
#include <QSerialPort>

class serialcom : public QObject
{
    Q_OBJECT
private:
    QSerialPort*port;
public:
    explicit serialcom(QObject *parent = nullptr,QSerialPort*port=nullptr);
signals:
    void dataReceived(const QByteArray &data);
    void _error(const QString&error);
private slots:
    void handleReadReady();
    void handleError(QSerialPort::SerialPortError error);
};

#endif // SERIALCOM_H
