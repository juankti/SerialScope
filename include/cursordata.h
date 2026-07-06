#ifndef CURSORDATA_H
#define CURSORDATA_H

#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include "qcustomplot.h"

struct cursors{
    QCPItemStraightLine*line=nullptr;
    QString text;
    float yVal;
    float xVal;
};

class cursordata : public QDialog
{
    Q_OBJECT

public:
    explicit cursordata(QWidget *parent = nullptr);
    ~cursordata();

    cursors curs_vec[2];
    int m_currentCursorIdx;

    void setdata();
    void formatTimeLabel(double val, QLabel* valLabel, QLabel* unitLabel);
    void updateValue(double tVal, double yVal);

private:
    QLabel *lblT1_val, *lblT1_unit;
    QLabel *lblV1_val, *lblV1_unit;
    QLabel *lblT2_val, *lblT2_unit;
    QLabel *lblV2_val, *lblV2_unit;
    QLabel *lblDT_val, *lblDT_unit;
    QLabel *lblDV_val, *lblDV_unit;
    QLabel *lblFreq_val, *lblFreq_unit;
};

#endif // CURSORDATA_H
