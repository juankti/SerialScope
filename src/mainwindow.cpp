#include "mainwindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentTime(0.0)
    , m_isConnected(false)
{
    ui->setupUi(this);
    ui->grafica->setInteraction(QCP::iRangeDrag,false);
    ui->grafica->setInteraction(QCP::iRangeZoom,true);

    m_ringBuffer = new ringbuffer(100000);
    m_ringBufferCH2 = new ringbuffer(100000);

    m_serialHandler = new serialhandler(m_ringBuffer, m_ringBufferCH2, this);

    m_renderTimer = new QTimer(this);
    connect(m_renderTimer, &QTimer::timeout, this, &MainWindow::updatePlot);

    ui->grafica->addGraph(); // CH1
    ui->grafica->graph(0)->setPen(QPen(Qt::green)); 
    ui->grafica->addGraph(); // CH2
    ui->grafica->graph(1)->setPen(QPen(Qt::yellow)); 

    ui->grafica->xAxis->setLabel("Time (s)");
    ui->grafica->yAxis->setLabel("Voltage (V)");
    ui->grafica->yAxis->setRange(0, 5.5);

    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setWidth(1);
    gridPen.setColor(Qt::lightGray);
    ui->grafica->xAxis->grid()->setPen(gridPen);
    ui->grafica->yAxis->grid()->setPen(gridPen);
    ui->grafica->xAxis->grid()->setAntialiased(true);
    ui->grafica->yAxis->grid()->setAntialiased(true);

    v_cursor = new QCPItemLine(ui->grafica);
    v_cursor->setPen(QPen(Qt::red, 1, Qt::DashLine));
    v_cursor->setVisible(false);

    h_cursor = new QCPItemLine(ui->grafica);
    h_cursor->setPen(QPen(Qt::red, 1, Qt::DashLine));
    h_cursor->setVisible(false);

    for (int i = 0; i < 2; ++i) {
        m_cursors[i] = new QCPItemStraightLine(ui->grafica);
        m_cursors[i]->setPen(QPen(Qt::red, 1, Qt::DashLine));
        m_cursors[i]->setVisible(false);

        m_cursorLabels[i] = new QCPItemText(ui->grafica);
        m_cursorLabels[i]->setColor(Qt::red);
        m_cursorLabels[i]->setVisible(false);
    }

    connect(ui->grafica, &QCustomPlot::mousePress, this, &MainWindow::onGraphClicked);
    
    // Connect Pause button
    connect(ui->btnPause, &QPushButton::clicked, this, &MainWindow::on_btnPause_clicked);
}

MainWindow::~MainWindow()
{
    delete m_serialHandler;
    delete m_ringBuffer;
    delete m_ringBufferCH2;
    delete ui;
}


void MainWindow::updatePlot()
{
    // View settings
    QString tdivStr = ui->comboTimeDiv->currentText();
    double windowSize = 2.0; // Default
    if (tdivStr == "100 us") windowSize = 0.0001 * 10;
    else if (tdivStr == "200 us") windowSize = 0.0002 * 10;
    else if (tdivStr == "500 us") windowSize = 0.0005 * 10;
    else if (tdivStr == "1 ms") windowSize = 0.001 * 10;
    else if (tdivStr == "2 ms") windowSize = 0.002 * 10;
    else if (tdivStr == "5 ms") windowSize = 0.005 * 10;
    else if (tdivStr == "10 ms") windowSize = 0.010 * 10;
    else if (tdivStr == "20 ms") windowSize = 0.020 * 10;
    else if (tdivStr == "50 ms") windowSize = 0.050 * 10;
    else if (tdivStr == "100 ms") windowSize = 0.100 * 10;
    else if (tdivStr == "200 ms") windowSize = 0.200 * 10;
    else if (tdivStr == "500 ms") windowSize = 0.500 * 10;
    else if (tdivStr == "1 s") windowSize = 1.0 * 10;
    
    QString vdivStr = ui->comboVoltsDiv->currentText();
    double vdiv = 1.0;
    if (vdivStr.startsWith("0.1")) vdiv = 0.1;
    else if (vdivStr.startsWith("0.2")) vdiv = 0.2;
    else if (vdivStr.startsWith("0.5")) vdiv = 0.5;
    else if (vdivStr.startsWith("1")) vdiv = 1.0;
    else if (vdivStr.startsWith("2")) vdiv = 2.0;
    
    double offset = ui->spinYOffset->value();
    double vref = m_pConfigDlg ? m_pConfigDlg->getVRef() : 5.0;
    double top = vref - offset;
    ui->grafica->yAxis->setRange(top - vdiv * 8, top);
    
    bool dualChannel = m_pConfigDlg && m_pConfigDlg->isDualChannel();
    
    std::vector<double> incomingRaw = m_ringBuffer->getAndClear();
    std::vector<double> incomingRaw2 = m_ringBufferCH2->getAndClear();

    if (m_isPaused) {
        ui->grafica->replot();
        return;
    }
    if (incomingRaw.empty()) {
        ui->grafica->replot();
        return;
    }
    
    float sampleRate = 1000000.0/10.0; // Default
    if (m_pConfigDlg) {
        sampleRate = m_pConfigDlg->getBaud() / 10.0;
        if (dualChannel) sampleRate /= 2.0;
    }
    const double TIME_STEP = 1.0 / sampleRate;


    QString trigModeStr = ui->comboTrigMode->currentText();
    bool isRolling = (trigModeStr == "Rolling");
    
    if (isRolling) {
        if (m_triggerState != ROLLING) {
            m_triggerState = ROLLING;
            ui->grafica->graph(0)->data()->clear();
            if (dualChannel) ui->grafica->graph(1)->data()->clear();
            m_currentTime = 0;
        }
        
        for (size_t i = 0; i < incomingRaw.size(); ++i) {
            m_currentTime += TIME_STEP;
            ui->grafica->graph(0)->addData(m_currentTime, incomingRaw[i]);
            if (dualChannel && i < incomingRaw2.size()) {
                ui->grafica->graph(1)->addData(m_currentTime, incomingRaw2[i]);
            }
        }
        
        if (!dualChannel) {
            ui->grafica->graph(1)->data()->clear();
        }
        
        ui->grafica->graph(0)->data()->removeBefore(m_currentTime - windowSize * 1.5);
        if (dualChannel) {
            ui->grafica->graph(1)->data()->removeBefore(m_currentTime - windowSize * 1.5);
        }
        
        ui->grafica->xAxis->setRange(m_currentTime - windowSize, m_currentTime);
        ui->grafica->replot();
        updateMeasurements();
        
    } else {
        // Trigger Mode
        if (m_triggerState == ROLLING) {
            m_triggerState = WAITING;
            m_trigX1.clear(); m_trigY1.clear();
            m_trigX2.clear(); m_trigY2.clear();
        }
        
        m_samplesNeeded = (int)(windowSize / TIME_STEP);
        bool isAuto = (trigModeStr == "Auto");
        bool isRising = (ui->comboTrigEdge->currentText() == "Rising");
        double trigLevel = ui->spinTrigLevel->value();
        
        bool frameComplete = false;
        
        for (size_t i = 0; i < incomingRaw.size(); ++i) {
            double val = incomingRaw[i];
            
            if (m_triggerState == WAITING) {
                bool triggered = false;
                if (isAuto && m_trigX1.size() == 0) {
                    triggered = true;
                } else if (isRising && m_lastTriggerVolt < trigLevel && val >= trigLevel) {
                    triggered = true;
                } else if (!isRising && m_lastTriggerVolt > trigLevel && val <= trigLevel) {
                    triggered = true;
                }
                
                if (triggered) {
                    m_triggerState = COLLECTING;
                    m_trigX1.clear(); m_trigY1.clear();
                    m_trigX2.clear(); m_trigY2.clear();
                    m_samplesCollected = 0;
                }
            }
            
            if (m_triggerState == COLLECTING) {
                m_trigX1.append(m_samplesCollected * TIME_STEP);
                m_trigY1.append(val);
                if (dualChannel && i < incomingRaw2.size()) {
                    m_trigX2.append(m_samplesCollected * TIME_STEP);
                    m_trigY2.append(incomingRaw2[i]);
                }
                
                m_samplesCollected++;
                if (m_samplesCollected >= m_samplesNeeded) {
                    frameComplete = true;
                    break;
                }
            }
            m_lastTriggerVolt = val;
        }
        
        if (frameComplete) {
            ui->grafica->graph(0)->setData(m_trigX1, m_trigY1);
            if (dualChannel) {
                ui->grafica->graph(1)->setData(m_trigX2, m_trigY2);
            } else {
                ui->grafica->graph(1)->data()->clear();
            }
            ui->grafica->xAxis->setRange(0, windowSize);
            ui->grafica->replot();
            
            updateMeasurements();
            
            m_triggerState = WAITING; // Ready for next frame
        }
    }
}

void MainWindow::updateMeasurements() {
    auto data = ui->grafica->graph(0)->data();
    if (data->isEmpty()) return;
    
    double lowerBound = ui->grafica->xAxis->range().lower;
    auto it = data->findBegin(lowerBound);
    auto end = data->end();
    
    if (it == end) return;
    
    double max_v = it->value;
    double min_v = it->value;
    int count = 0;
    
    for (auto i = it; i != end; ++i) {
        double v = i->value;
        if (v > max_v) max_v = v;
        if (v < min_v) min_v = v;
        count++;
    }
    
    double mid = (max_v + min_v) / 2.0;
    int crossings = 0;
    double prev_v = it->value;
    
    double first_cross_time = -1.0;
    double last_cross_time = -1.0;
    
    for (auto i = it; i != end; ++i) {
        double v = i->value;
        double t = i->key;
        if (prev_v <= mid && v > mid) {
            crossings++;
            if (first_cross_time < 0) first_cross_time = t;
            last_cross_time = t;
        }
        prev_v = v;
    }
    
    ui->labVmax->setText(QString("Vmax: %1 V").arg(max_v, 0, 'f', 2));
    ui->labVmin->setText(QString("Vmin: %1 V").arg(min_v, 0, 'f', 2));
    ui->labVpp->setText(QString("Vpp: %1 V").arg(max_v - min_v, 0, 'f', 2));
    
    if (crossings > 1 && last_cross_time > first_cross_time) {
        double actual_span = last_cross_time - first_cross_time;
        double freq = (crossings - 1) / actual_span;
        ui->labFreq->setText(QString("Freq: %1 Hz").arg(freq, 0, 'f', 1));
    } else {
        ui->labFreq->setText("Freq: -- Hz");
    }
}

void MainWindow::on_btnPause_clicked()
{
    m_isPaused = ui->btnPause->isChecked();
}

void MainWindow::on_btnSettings_clicked()
{
    m_pConfigDlg = new portconfig(this, m_serialHandler->rawPort());
    m_pConfigDlg->exec();
}

void MainWindow::on_btnConnect_clicked()
{
    QSerialPort* port = m_serialHandler->rawPort();

    if (port->portName().isEmpty()) {
        on_btnSettings_clicked();
        if (port->portName().isEmpty()) return;
    }

    int baud=1000000; //default

    if(m_pConfigDlg){
        baud=m_pConfigDlg->getBaud();
        m_serialHandler->setVRef(m_pConfigDlg->getVRef());
        m_serialHandler->setDualChannel(m_pConfigDlg->isDualChannel());
    }

    m_serialHandler->connectPort(port->portName(), baud);

    if (port->isOpen()) {
        m_isConnected = true;
        m_renderTimer->start();
        m_currentTime=0.0;
        m_plotData.clear();
        m_ringBuffer->clear();
        m_ringBufferCH2->clear();
        
        m_triggerState = WAITING;

        ui->btnConnect->setEnabled(false);
        ui->btnDisconnect->setEnabled(true);
        ui->btnSettings->setEnabled(false);

        ui->labOpenPort->setText("Connected to " + port->portName() + " @ " + QString::number(baud));
        ui->labOpenPort->setStyleSheet("color: green; font-weight: bold;");
    } else {
        ui->labOpenPort->setText("Connection Failed!");
        ui->labOpenPort->setStyleSheet("color: red;");
    }
}
void MainWindow::on_btnDisconnect_clicked()
{
    m_serialHandler->disconnectPort();
    m_renderTimer->stop();
    m_isConnected = false;
    ui->btnConnect->setEnabled(true);
    ui->btnDisconnect->setEnabled(false);
}

void MainWindow::on_btnGraphOptions_clicked()
{
   if(!m_pGraphOptDlg){
		m_pGraphOptDlg = new graphoptions(this);

		connect(m_pGraphOptDlg, &graphoptions::applyChanges, [=](){
            applyGraphSettings();
        });
    }
	m_pGraphOptDlg->exec();
}

void MainWindow::on_btnExport_clicked() {
    auto graphData = ui->grafica->graph(0)->data();

    QVector<double> qtTime, qtVolts;

    for (auto it = graphData->begin(); it != graphData->end(); ++it) {
        qtTime.append(it->key);   // The time X-value
        qtVolts.append(it->value); // The voltage Y-value
    }

    exportdlg dlg(this,ui->grafica,&qtTime,&qtVolts,m_pGraphOptDlg);
    dlg.exec();
}

void MainWindow::applyGraphSettings()
{
    if (!m_pGraphOptDlg) return;

    ui->grafica->xAxis->grid()->setVisible(m_pGraphOptDlg->grid());
    ui->grafica->yAxis->grid()->setVisible(m_pGraphOptDlg->grid());

    ui->grafica->yAxis->setScaleType(m_pGraphOptDlg->scale());

    ui->grafica->xAxis->setLabel(m_pGraphOptDlg->xLab());
    ui->grafica->yAxis->setLabel(m_pGraphOptDlg->yLab());

    if (!m_pGraphOptDlg->autoFit()) {
        // Custom range
        double tMin = m_pGraphOptDlg->tMin();
        double tMax = m_pGraphOptDlg->tMax();

        if (tMin >= 0 && tMax >= 0) { // Valid range check
            ui->grafica->xAxis->setRange(tMin, tMax);
        }
    }
    // If autoFit is true, the range is already handled by updatePlot()

    ui->grafica->replot();
}

void MainWindow::onGraphClicked(QMouseEvent *event)
{
    if (!ui->checkCursors->isChecked()) return;
    if (ui->grafica->graphCount() == 0) return;

    double xVal = ui->grafica->xAxis->pixelToCoord(event->pos().x());

    if (m_cursors[m_cursorIdx]) {
        m_cursors[m_cursorIdx]->point1->setCoords(xVal, 0);
        m_cursors[m_cursorIdx]->point2->setCoords(xVal, 5);
        m_cursors[m_cursorIdx]->setVisible(true);
    }

    double yTop = ui->grafica->yAxis->range().upper;
    if (m_cursorLabels[m_cursorIdx]) {
        m_cursorLabels[m_cursorIdx]->setText(QString::number(m_cursorIdx+1));
        m_cursorLabels[m_cursorIdx]->position->setCoords(xVal + 0.05, yTop * 0.8);
        m_cursorLabels[m_cursorIdx]->setVisible(true);
    }
    double voltage = 0.0;
    auto dataContainer = ui->grafica->graph(0)->data();
    auto it = dataContainer->findBegin(xVal);
    if(it != dataContainer->end()) voltage = it->value;

    qDebug() << "Cursor" << m_cursorIdx + 1 << ": Time=" << xVal << "s, Volt=" << voltage << "V";

    if (m_cursorIdx == 1 && !m_pCursorDlg) {
        m_pCursorDlg = new cursordata(this);
    }

    if (m_pCursorDlg && m_cursorIdx == 1 && !m_pCursorDlg->isVisible()) {
        m_pCursorDlg->show();
    }

    if (m_pCursorDlg) {
        m_pCursorDlg->m_currentCursorIdx = m_cursorIdx;
        m_pCursorDlg->updateValue(xVal, voltage);
    }

    m_cursorIdx = (m_cursorIdx + 1) % 2;
    ui->grafica->replot();
}


void MainWindow::on_checkCursors_checkStateChanged(const Qt::CheckState & /*arg1*/)
{
    if(ui->checkCursors->checkState()==0){
        v_cursor->setVisible(false);
        h_cursor->setVisible(false);
        for (auto a : m_cursors) a->setVisible(false);
    }
}

