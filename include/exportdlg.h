#ifndef EXPORTDLG_H
#define EXPORTDLG_H

#include <QDialog>
#include "qcustomplot.h"
#include <QBuffer>
#include <QFileDialog>
#include "graphoptions.h"

namespace Ui {
class exportdlg;
}

class exportdlg : public QDialog
{
    Q_OBJECT

public:
    explicit exportdlg(QWidget *parent = nullptr,
                       QCustomPlot*plot=nullptr,
                       QVector<double>* data=nullptr,
                       QVector<double>* y=nullptr,
                       graphoptions*grOp=nullptr);
    void exportImage();
    ~exportdlg();

private slots:
    void on_btnExport_clicked();
    void on_qualitySlider_valueChanged(int value);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::exportdlg *ui;
    QVector<double>*data;
    QVector<double>* y;
    QString folderpath;
    QPixmap pixmap;
    QCustomPlot*plot;
    graphoptions*grOps;
};

#endif //
