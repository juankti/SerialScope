#ifndef PORTCONFIG_H
#define PORTCONFIG_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QListWidget>
#include <QMetaEnum>

namespace Ui {
class portconfig;
}

class portconfig : public QDialog
{
    Q_OBJECT

public:
    explicit portconfig(QWidget *parent = nullptr,QSerialPort*port=nullptr);
    ~portconfig();
    void portSelected(QListWidgetItem*item);
    void viewconfigs(bool);
    int getBaud();
private slots:
    void on_btnPorts_clicked();
    void on_pushButton_clicked();

private:
    Ui::portconfig *ui;
    QList<QSerialPortInfo> devices;
    QSerialPort*port;
};

#endif // PORTCONFIG_H
