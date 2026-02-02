#ifndef CURSOR_H
#define CURSOR_H

#include "qcustomplot.h"

struct cursors{
    QCPItemStraightLine* line=nullptr;
    QString text;
    int yVal;
    float xVal;
};

class cursor{
public:
    cursor(){};
    cursors curs_vec[2];
    ~cursor();
};

#endif // CURSOR_H
