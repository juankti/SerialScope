#include "cursordata.h"
#include "ui_cursordata.h"

cursordata::cursordata(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::cursordata)
{
    ui->setupUi(this);
    setWindowTitle("Cursor Measurement");
    m_currentCursorIdx = 0;
}


void cursordata::setdata(){
    ui->label_2->setText(QString::number(curs_vec[0].xVal));
    ui->label_8->setText(QString::number(curs_vec[0].yVal));
    ui->label_38->setText(QString::number(curs_vec[1].xVal));
    ui->label_41->setText(QString::number(curs_vec[1].yVal));

    ui->label_14->setText(QString::number(curs_vec[1].xVal-curs_vec[0].xVal));
    ui->label_26->setText(QString::number(curs_vec[1].yVal-curs_vec[0].yVal));
    ui->label_29->setText(QString::number(1/(curs_vec[1].xVal-curs_vec[0].xVal)));

}

void cursordata::updateValue(double tVal, double yVal)
{
    curs_vec[m_currentCursorIdx].xVal = (float)tVal;
    curs_vec[m_currentCursorIdx].yVal = (int)yVal; //

    setdata();

    m_currentCursorIdx = (m_currentCursorIdx + 1) % 2;
}

cursordata::~cursordata()
{
    delete ui;
}
