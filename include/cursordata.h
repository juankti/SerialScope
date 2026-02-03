#ifndef CURSORDATA_H
#define CURSORDATA_H

#include <QDialog>
#include "qcustomplot.h"

namespace Ui {
class cursordata;
}


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
     cursordata(QWidget *parent = nullptr);
    cursors curs_vec[2];
    void setdata();
    void updateValue(double tVal, double yVal);
    int m_currentCursorIdx;
    ~cursordata();

private:
    Ui::cursordata *ui;
};

#endif // CURSORDATA_H
