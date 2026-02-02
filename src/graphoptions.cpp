#include "graphoptions.h"
#include "ui_graphoptions.h"


graphoptions::graphoptions(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::graphoptions)
{
    ui->setupUi(this);
    setWindowTitle("Graph Options");
    setup();
    setLabelsVis(false);
    setRangeVis(false);
    ui->leTWnd->setVisible(false);
    ui->label_7->setVisible(false);
    if(autoFit()) setTWnd(2);
    ui->checkGrid->setChecked(true);
}

void graphoptions::setLabelsVis(bool visible){
    ui->label_15->setVisible(visible);
    ui->label_16->setVisible(visible);
    ui->label_17->setVisible(visible);
    ui->leXLab->setVisible(visible);
    ui->leYLab->setVisible(visible);
}

void graphoptions::setRangeVis(bool visible){
    ui->label_3->setVisible(visible);
    ui->label_6->setVisible(visible);
    ui->label_8->setVisible(visible);
    ui->leTMin->setVisible(visible);
    ui->leTMax->setVisible(visible);

}

void graphoptions::setup(){
    ui->comboScale->addItem("Linear");
    ui->comboScale->addItem("Log");
    ui->comboRange->addItem("Auto");
    ui->comboRange->addItem("Custom...");
}

graphoptions::~graphoptions()
{
    delete ui;
}

QString graphoptions::yLab(){
    if (ui->checkGrid_3->isChecked() &&ui->leYLab->text().isEmpty()) QMessageBox::warning(this,"","Set axis name");
    if (ui->checkGrid_3->isChecked() &&ui->leXLab->text().isEmpty()) QMessageBox::warning(this,"","Set axis name");
    return ui->leYLab->text();
}

QString graphoptions::xLab(){
    if (ui->checkGrid_3->isChecked() &&ui->leYLab->text().isEmpty()) QMessageBox::warning(this,"","Set axis name");
    if (ui->checkGrid_3->isChecked() &&ui->leXLab->text().isEmpty()) QMessageBox::warning(this,"","Set axis name");
    return ui->leXLab->text();
}

double graphoptions::tMin(){
    if (ui->leTMin->text().isEmpty() || ui->leTMax->text().isEmpty()
        || ui->leTMin->text().toDouble()>ui->leTMax->text().toDouble()){
        QMessageBox::warning(this,"","Insert valid bounds.");
        return -1;
    }
    return ui->leTMin->text().toDouble();
}

double graphoptions::tMax(){
    if (ui->leTMin->text().isEmpty() || ui->leTMax->text().isEmpty()
        ||ui->leTMin->text().toDouble()>ui->leTMax->text().toDouble()){
        QMessageBox::warning(this,"","Insert valid bounds.");
        return -1;
    }
    return ui->leTMax->text().toDouble();
}

QCPAxis::ScaleType graphoptions::scale(){
    if (ui->comboScale->currentText()=="Linear") return QCPAxis::ScaleType::stLinear;
    else return QCPAxis::ScaleType::stLogarithmic;
}


bool graphoptions::grid(){
    return ui->checkGrid->isChecked();
}

bool graphoptions::autoFit(){
    return (ui->comboRange->currentText()=="Auto");
}

void graphoptions::on_btnSet_clicked()
{
    if(!ui->comboRange->currentText().isEmpty() &&
        !ui->comboScale->currentText().isEmpty()){
        setTWnd(2);
        emit applyChanges();
        this->close();
    }else{
        QMessageBox::warning(this,"","Please select scale/range.");
    }

}


void graphoptions::on_checkGrid_3_toggled(bool checked)
{
    setLabelsVis(checked);
}


void graphoptions::setTWnd(double a){
    if (autoFit()){
        setRangeVis(false);
        ui->leTWnd->setVisible(true);
        ui->label_7->setVisible(true);
        if(!ui->leTWnd->text().isEmpty()){
            timeWnd=ui->leTWnd->text().toDouble();
        } else timeWnd=a;
    } else timeWnd=a;
}

void graphoptions::on_comboRange_currentTextChanged(const QString &arg1)
{
    if (autoFit()){
        ui->leTWnd->setVisible(autoFit());
        ui->label_7->setVisible(autoFit());
        setRangeVis(!autoFit());
    } else{
        ui->leTWnd->setVisible(autoFit());
        ui->label_7->setVisible(autoFit());
        setRangeVis(!autoFit());
    }

}


