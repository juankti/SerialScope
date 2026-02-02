#ifndef GRAPHOPTIONS_H
#define GRAPHOPTIONS_H

#include <QDialog>
#include "qcustomplot.h"

namespace Ui {
class graphoptions;
}

class graphoptions : public QDialog
{
    Q_OBJECT
public:
    explicit graphoptions(QWidget *parent = nullptr);
    void setup();
    void setLabelsVis(bool visible);
    void setRangeVis(bool visible);
    bool grid();
    bool autoFit();
    double tMin();
    double tMax();
    double timeWnd=2;
    void setTWnd(double _default);
    QCPAxis::ScaleType scale();
    QString yLab();
    QString xLab();
    ~graphoptions();
signals:
    void applyChanges();
private slots:
    void on_btnSet_clicked();

    void on_checkGrid_3_toggled(bool checked);
    void on_comboRange_currentTextChanged(const QString &arg1);

private:
    Ui::graphoptions *ui;
};

#endif // GRAPHOPTIONS_H
