#include "cursordata.h"
#include <QFrame>

cursordata::cursordata(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Cursor Measurements");
    setMinimumSize(320, 200);
    m_currentCursorIdx = 0;
    
    // Setup UI programmatically
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Style
    setStyleSheet("QDialog { background-color: #f8f9fa; }"
                  "QLabel { font-size: 10pt; color: #212529; }"
                  "QLabel[title=\"true\"] { font-weight: bold; color: #495057; }"
                  "QLabel[val=\"true\"] { font-family: monospace; font-size: 11pt; color: #0d6efd; }"
                  "QLabel[delta=\"true\"] { font-family: monospace; font-size: 12pt; font-weight: bold; color: #dc3545; }"
                  "QFrame#line { background-color: #dee2e6; }");
    
    // Helper lambda to create titles
    auto createTitle = [](const QString &text) {
        QLabel *lbl = new QLabel(text);
        lbl->setProperty("title", true);
        lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        return lbl;
    };
    
    // Initialize labels
    lblT1_val = new QLabel("0.00"); lblT1_val->setProperty("val", true); lblT1_val->setAlignment(Qt::AlignRight);
    lblT1_unit = new QLabel("us");  lblT1_unit->setProperty("val", true);
    
    lblV1_val = new QLabel("0.00"); lblV1_val->setProperty("val", true); lblV1_val->setAlignment(Qt::AlignRight);
    lblV1_unit = new QLabel("V");   lblV1_unit->setProperty("val", true);
    
    lblT2_val = new QLabel("0.00"); lblT2_val->setProperty("val", true); lblT2_val->setAlignment(Qt::AlignRight);
    lblT2_unit = new QLabel("us");  lblT2_unit->setProperty("val", true);
    
    lblV2_val = new QLabel("0.00"); lblV2_val->setProperty("val", true); lblV2_val->setAlignment(Qt::AlignRight);
    lblV2_unit = new QLabel("V");   lblV2_unit->setProperty("val", true);
    
    lblDT_val = new QLabel("0.00"); lblDT_val->setProperty("delta", true); lblDT_val->setAlignment(Qt::AlignRight);
    lblDT_unit = new QLabel("us");  lblDT_unit->setProperty("delta", true);
    
    lblDV_val = new QLabel("0.00"); lblDV_val->setProperty("delta", true); lblDV_val->setAlignment(Qt::AlignRight);
    lblDV_unit = new QLabel("V");   lblDV_unit->setProperty("delta", true);
    
    lblFreq_val = new QLabel("0.00"); lblFreq_val->setProperty("delta", true); lblFreq_val->setAlignment(Qt::AlignRight);
    lblFreq_unit = new QLabel("Hz");  lblFreq_unit->setProperty("delta", true);
    
    // Layout Cursor 1
    mainLayout->addWidget(createTitle("t1:"), 0, 0);
    mainLayout->addWidget(lblT1_val, 0, 1);
    mainLayout->addWidget(lblT1_unit, 0, 2);
    
    mainLayout->addWidget(createTitle("y1:"), 0, 3);
    mainLayout->addWidget(lblV1_val, 0, 4);
    mainLayout->addWidget(lblV1_unit, 0, 5);
    
    // Layout Cursor 2
    mainLayout->addWidget(createTitle("t2:"), 1, 0);
    mainLayout->addWidget(lblT2_val, 1, 1);
    mainLayout->addWidget(lblT2_unit, 1, 2);
    
    mainLayout->addWidget(createTitle("y2:"), 1, 3);
    mainLayout->addWidget(lblV2_val, 1, 4);
    mainLayout->addWidget(lblV2_unit, 1, 5);
    
    // Separator line
    QFrame *line = new QFrame();
    line->setObjectName("line");
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line, 2, 0, 1, 6);
    
    // Layout Deltas
    mainLayout->addWidget(createTitle("Δt:"), 3, 0);
    mainLayout->addWidget(lblDT_val, 3, 1);
    mainLayout->addWidget(lblDT_unit, 3, 2);
    
    mainLayout->addWidget(createTitle("Δy:"), 3, 3);
    mainLayout->addWidget(lblDV_val, 3, 4);
    mainLayout->addWidget(lblDV_unit, 3, 5);
    
    mainLayout->addWidget(createTitle("1/Δt:"), 4, 0);
    mainLayout->addWidget(lblFreq_val, 4, 1);
    mainLayout->addWidget(lblFreq_unit, 4, 2);
}

cursordata::~cursordata()
{
}

void cursordata::formatTimeLabel(double val, QLabel* valLabel, QLabel* unitLabel) {
    double absVal = std::abs(val);
    if (absVal < 1e-3) {
        valLabel->setText(QString::number(val * 1e6, 'f', 2));
        unitLabel->setText("us");
    } else if (absVal >= 1e-3 && absVal < 1.0) {
        valLabel->setText(QString::number(val * 1e3, 'f', 2));
        unitLabel->setText("ms");
    } else {
        valLabel->setText(QString::number(val, 'f', 3));
        unitLabel->setText("s");
    }
}

void cursordata::setdata(){
    formatTimeLabel(curs_vec[0].xVal, lblT1_val, lblT1_unit);
    lblV1_val->setText(QString::number(curs_vec[0].yVal, 'f', 3));
    
    formatTimeLabel(curs_vec[1].xVal, lblT2_val, lblT2_unit);
    lblV2_val->setText(QString::number(curs_vec[1].yVal, 'f', 3));

    double dt = curs_vec[1].xVal - curs_vec[0].xVal;
    formatTimeLabel(dt, lblDT_val, lblDT_unit);
    lblDV_val->setText(QString::number(curs_vec[1].yVal - curs_vec[0].yVal, 'f', 3));
    
    if (std::abs(dt) > 1e-12) {
        lblFreq_val->setText(QString::number(std::abs(1.0 / dt), 'f', 2));
    } else {
        lblFreq_val->setText("inf");
    }
}

void cursordata::updateValue(double tVal, double yVal)
{
    curs_vec[m_currentCursorIdx].xVal = (float)tVal;
    curs_vec[m_currentCursorIdx].yVal = (float)yVal; //

    setdata();
}
