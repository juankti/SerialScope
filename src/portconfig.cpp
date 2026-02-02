#include "portconfig.h"
#include "ui_portconfig.h"
#include <QMessageBox>


portconfig::portconfig(QWidget *parent,QSerialPort*port)
    : QDialog(parent)
    , ui(new Ui::portconfig)
{
    ui->setupUi(this);
    setWindowTitle("Port Configuration");
    this->port=port;
    viewconfigs(false);

    QMetaEnum dbenum = QMetaEnum::fromType<QSerialPort::DataBits>();
    for (int a = 0; a < dbenum.keyCount(); ++a) {
        ui->comboBaud_2->addItem(QString(dbenum.key(a)));
    }

    QMetaEnum parenum = QMetaEnum::fromType<QSerialPort::Parity>();
    for (int a = 0; a < parenum.keyCount(); ++a) {
        ui->comboBaud_3->addItem(QString(parenum.key(a)));
    }

    QMetaEnum stopenum = QMetaEnum::fromType<QSerialPort::StopBits>();
    for (int a = 0; a < stopenum.keyCount(); ++a) {
        ui->comboBaud_4->addItem(QString(stopenum.key(a)));
    }

    QMetaEnum flownum = QMetaEnum::fromType<QSerialPort::FlowControl>();
    for (int a = 0; a < flownum.keyCount(); ++a) {
        ui->comboBaud_5->addItem(QString(flownum.key(a)));
    }
}

portconfig::~portconfig()
{
    delete ui;
}

void portconfig::on_btnPorts_clicked()
{
    ui->portsList->clear();
    devices = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo&ports : devices)
        ui->portsList->addItem(ports.portName()+" "+ports.description());
    connect(ui->portsList,&QListWidget::itemDoubleClicked,this,&portconfig::portSelected);
}

void portconfig::viewconfigs(bool visible){
    ui->label->setVisible(visible);
    ui->label_2->setVisible(visible);
    ui->label_3->setVisible(visible);
    ui->label_4->setVisible(visible);
    ui->label_5->setVisible(visible);
    ui->label_6->setVisible(visible);
    ui->leBaud->setVisible(visible);
    ui->comboBaud_2->setVisible(visible);
    ui->comboBaud_3->setVisible(visible);
    ui->comboBaud_4->setVisible(visible);
    ui->comboBaud_5->setVisible(visible);
    ui->pushButton->setVisible(visible);
}

void portconfig::portSelected(QListWidgetItem*item){
    viewconfigs(true);
    ui->label_2->setText(devices.at(ui->portsList->row(item)).portName());
}



void portconfig::on_pushButton_clicked()
{
    if (!ui->leBaud->text().isEmpty()
        &&ui->comboBaud_2->currentIndex()!=-1
        &&ui->comboBaud_3->currentIndex()!=-1
        &&ui->comboBaud_4->currentIndex()!=-1
        &&ui->comboBaud_5->currentIndex()!=-1){
        port->setPortName(ui->label_2->text());
        port->setBaudRate(ui->leBaud->text().toInt());
        QMetaEnum dbenum = QMetaEnum::fromType<QSerialPort::DataBits>();
        port->setDataBits(static_cast<QSerialPort::DataBits>(
            dbenum.keyToValue(ui->comboBaud_2->currentText().toLatin1().data())
            ));
        QMetaEnum parenum = QMetaEnum::fromType<QSerialPort::Parity>();
        port->setParity(static_cast<QSerialPort::Parity>(
            parenum.keyToValue(ui->comboBaud_3->currentText().toLatin1().data())
            ));
       QMetaEnum stopenum = QMetaEnum::fromType<QSerialPort::StopBits>();
        port->setStopBits(static_cast<QSerialPort::StopBits>(
            stopenum.keyToValue(ui->comboBaud_4->currentText().toLatin1().data())
            ));
        QMetaEnum flownum = QMetaEnum::fromType<QSerialPort::FlowControl>();
        port->setFlowControl(static_cast<QSerialPort::FlowControl>(
            flownum.keyToValue(ui->comboBaud_5->currentText().toLatin1().data())
            ));
            ;
            if (port->open(QIODevice::ReadWrite)){
                    QMessageBox::information(this,"",port->portName() + " opened.");
                    this->close();
                } else{
                    QMessageBox::warning(this,"","Could not open "+ port->portName()+".");
        }
    }
}


int portconfig::getBaud(){
    return ui->leBaud->text().toInt();
}



