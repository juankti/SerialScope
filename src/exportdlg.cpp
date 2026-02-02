#include "exportdlg.h"
#include "ui_exportdlg.h"

exportdlg::exportdlg(QWidget *parent,QCustomPlot*plot,QVector<double>*data,QVector<double>*y,graphoptions*grOp)
    : QDialog(parent)
    , ui(new Ui::exportdlg)
{
    ui->setupUi(this);
    setWindowTitle("Export graph");
    this->plot=plot;
    this->data=data;
    this->y=y;
    this->grOps=grOp;
}

exportdlg::~exportdlg()
{
    delete ui;
}

void exportdlg::on_btnExport_clicked()
{
    if(ui->tabWidget->currentIndex()==0){ // image export
        if (ui->leFilePath->text().isEmpty() ||  ui->leFileName->text().isEmpty()){
            QMessageBox::warning(this,"","Missing argument/s.");
        }else{
            QStringList dimensions = ui->comboDimensions->currentText().split("x");
            if (ui->comboRange->currentText()=="Current view"){
                pixmap=plot->toPixmap(dimensions[0].toInt(),dimensions[1].toInt(),1);
                QFile file(folderpath+"/"+ui->leFileName->text()+"."+ui->comboFormat->currentText());
                if (file.open(QIODevice::WriteOnly)){
                    pixmap.save(&file,ui->comboFormat->currentText().toUtf8().data(),ui->qualitySlider->value());
                    QMessageBox::information(this,"","Saved at "+folderpath+".");
                }else{
                    QMessageBox::warning(this,"","Could not open the file.");
                }
                file.close();
            }else{
                QCustomPlot* entirePlot= new QCustomPlot(this);
                entirePlot->addGraph();
                entirePlot->graph(0)->setData(*data,*y);
                if (data->isEmpty()) entirePlot->xAxis->setRange(0,1);
                else entirePlot->xAxis->setRange(data->first(),data->last());
                entirePlot->yAxis->setRange(0,1023);
                entirePlot->xAxis->setLabel(grOps->xLab());
                entirePlot->yAxis->setLabel(grOps->yLab());
                entirePlot->replot();
                pixmap=entirePlot->toPixmap(dimensions[0].toInt(),dimensions[1].toInt(),1);
                QFile file(folderpath+"/"+ui->leFileName->text()+"."+ui->comboFormat->currentText());
                if (file.open(QIODevice::WriteOnly)){
                    pixmap.save(&file,ui->comboFormat->currentText().toUtf8().data(),ui->qualitySlider->value());
                    QMessageBox::information(this,"","Saved at "+folderpath+".");
                }else{
                    QMessageBox::warning(this,"","Could not open the file.");
                }
                file.close();
                delete entirePlot;
            }
        }
    }else{ // data export
        if(ui->leFilePath_2->text().isEmpty() || ui->leFileName_2->text().isEmpty()){
            QMessageBox::warning(this,"","Missing argument/s.");
        } else{
            QString headers = "Index,xVal,yVal\n";
            if (ui->comboRange_2->currentText()=="Current view"){
                QFile file(folderpath+"/"+ui->leFileName_2->text()+"."+ui->comboFormat_2->currentText());
                if (file.open(QIODevice::WriteOnly)){
                    QString line;
                    file.write(headers.toUtf8());
                    for(int i=0;i<data->size();i++){
                        if (data->at(i)>= plot->xAxis->range().lower && data->at(i)<= plot->xAxis->range().upper){
                            line=QString::number(i+1)+","+QString::number(data->at(i))+","+QString::number(y->at(i))+"\n";
                            file.write(line.toUtf8());
                        }
                    }
                    QMessageBox::information(this,"","Saved at "+folderpath+".");
                }else{
                    QMessageBox::warning(this,"","Could not open the file.");
                }
                file.close();
            }else{
                QFile file(folderpath+"/"+ui->leFileName_2->text()+"."+ui->comboFormat_2->currentText());
                if (file.open(QIODevice::WriteOnly)){
                    QString line;
                    file.write(headers.toUtf8());
                    for(int i=0;i<data->size();i++){
                        line=QString::number(i+1)+","+QString::number(data->at(i))+","+QString::number(y->at(i))+"\n";
                        file.write(line.toUtf8());
                    }
                    QMessageBox::information(this,"","Saved at "+folderpath+".");
                }else{
                    QMessageBox::warning(this,"","Could not open the file.");
                }
                file.close();
            }
        }
    }
}


void exportdlg::on_qualitySlider_valueChanged(int value)
{
    ui->label_4->setText(QString::number(value));
}





void exportdlg::on_pushButton_2_clicked()
{
    folderpath=QFileDialog::getExistingDirectory(this,"",QApplication::applicationDirPath(),QFileDialog::ShowDirsOnly);
    ui->leFilePath->setText(folderpath);
}


void exportdlg::on_pushButton_3_clicked()
{
    folderpath=QFileDialog::getExistingDirectory(this,"",QApplication::applicationDirPath(),QFileDialog::ShowDirsOnly);
    ui->leFilePath_2->setText(folderpath);
}
