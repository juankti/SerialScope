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
    ui->grafica->setInteraction(QCP::iRangeDrag, false);
    ui->grafica->setInteraction(QCP::iRangeZoom, false);

    qRegisterMetaType<QVector<double>>("QVector<double>");

    m_fftWindow = new QMainWindow(this);
    m_fftWindow->setWindowTitle("FFT");
    m_fftWindow->resize(600, 400);
    m_fftPlot = new QCustomPlot(m_fftWindow);
    m_fftWindow->setCentralWidget(m_fftPlot);

    m_fftPlot->setInteraction(QCP::iRangeDrag, true);
    m_fftPlot->setInteraction(QCP::iRangeZoom, true);
    m_fftPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    m_fftPlot->axisRect()->setRangeZoom(Qt::Horizontal);

    m_rawBuffer = new rawringbuffer(100000);
    m_rawBufferCH2 = new rawringbuffer(100000);
    m_ringBuffer = new ringbuffer(100000);
    m_ringBufferCH2 = new ringbuffer(100000);
    m_ringBufferFilt = new ringbuffer(100000);
    m_ringBufferCH2Filt = new ringbuffer(100000);

    m_serialHandler = new serialhandler(m_rawBuffer, m_rawBufferCH2, this);

    m_dspPipeline = new DSPPipeline(m_rawBuffer, m_rawBufferCH2, 
                                    m_ringBuffer, m_ringBufferCH2, this);
    connect(m_dspPipeline, &DSPPipeline::fftDataReady, this, &MainWindow::onFftDataReady);

    m_renderTimer = new QTimer(this);
    connect(m_renderTimer, &QTimer::timeout, this, &MainWindow::updatePlot);

    ui->grafica->addGraph(); // CH1 RAW (0)
    ui->grafica->graph(0)->setPen(QPen(Qt::darkGreen)); 
    ui->grafica->addGraph(); // CH2 RAW (1)
    ui->grafica->graph(1)->setPen(QPen(Qt::darkYellow)); 
    
    ui->grafica->addGraph(); // CH1 FILT (2)
    ui->grafica->graph(2)->setPen(QPen(Qt::green, 1.5)); 
    ui->grafica->addGraph(); // CH2 FILT (3)
    ui->grafica->graph(3)->setPen(QPen(Qt::yellow, 1.5)); 

    ui->grafica->xAxis->setLabel("Time (s)");
    ui->grafica->yAxis->setLabel("Voltage (V)");
    ui->grafica->yAxis->setRange(0, 5.5);

    m_fftPlot->addGraph();
    m_fftPlot->graph(0)->setPen(QPen(Qt::magenta));
    m_fftPlot->addGraph();
    m_fftPlot->graph(1)->setPen(QPen(Qt::cyan));
    m_fftPlot->xAxis->setLabel("Frequency (Hz)");
    m_fftPlot->yAxis->setLabel("Magnitude (dB)");
    m_fftPlot->yAxis->setRange(-60, 60);

    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setWidth(1);
    gridPen.setColor(Qt::lightGray);
    ui->grafica->xAxis->grid()->setPen(gridPen);
    ui->grafica->yAxis->grid()->setPen(gridPen);
    ui->grafica->xAxis->grid()->setAntialiased(true);
    ui->grafica->yAxis->grid()->setAntialiased(true);
    
    m_fftPlot->xAxis->grid()->setPen(gridPen);
    m_fftPlot->yAxis->grid()->setPen(gridPen);

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

    // Initialize dial labels
    on_dialTimeDiv_valueChanged(ui->dialTimeDiv->value());
    on_dialVoltsDiv_valueChanged(ui->dialVoltsDiv->value());
    on_dialYOffset_valueChanged(ui->dialYOffset->value());

    ui->comboWindowType->setCurrentIndex(1); // Hanning
}

MainWindow::~MainWindow()
{
    if (m_dspPipeline) {
        m_dspPipeline->stop();
        delete m_dspPipeline;
    }
    delete m_serialHandler;
    delete m_rawBuffer;
    delete m_rawBufferCH2;
    delete m_ringBuffer;
    delete m_ringBufferCH2;
    delete ui;
}


void MainWindow::updatePlot()
{
    // View settings
    QStringList timeOptions = {"1 us", "2 us", "5 us", "10 us", "20 us", "50 us", "100 us", "200 us", "500 us", "1 ms", "2 ms", "5 ms", "10 ms", "20 ms", "50 ms", "100 ms", "200 ms", "500 ms", "1 s"};
    QString tdivStr = timeOptions[qBound(0, ui->dialTimeDiv->value(), timeOptions.size() - 1)];

    double windowSize = 2.0; // Default
    if (tdivStr == "1 us") windowSize = 0.000001 * 10;
    else if (tdivStr == "2 us") windowSize = 0.000002 * 10;
    else if (tdivStr == "5 us") windowSize = 0.000005 * 10;
    else if (tdivStr == "10 us") windowSize = 0.00001 * 10;
    else if (tdivStr == "20 us") windowSize = 0.00002 * 10;
    else if (tdivStr == "50 us") windowSize = 0.00005 * 10;
    else if (tdivStr == "100 us") windowSize = 0.0001 * 10;
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
    
    QStringList voltsOptions = {"0.1 V", "0.2 V", "0.5 V", "1 V", "2 V"};
    QString vdivStr = voltsOptions[qBound(0, ui->dialVoltsDiv->value(), voltsOptions.size() - 1)];

    double vdiv = 1.0;
    if (vdivStr.startsWith("0.1")) vdiv = 0.1;
    else if (vdivStr.startsWith("0.2")) vdiv = 0.2;
    else if (vdivStr.startsWith("0.5")) vdiv = 0.5;
    else if (vdivStr.startsWith("1")) vdiv = 1.0;
    else if (vdivStr.startsWith("2")) vdiv = 2.0;
    
    double offset = ui->dialYOffset->value() * 0.1;
    double vref = m_pConfigDlg ? m_pConfigDlg->getVRef() : 5.0;
    double top = vref - offset;
    ui->grafica->yAxis->setRange(top - vdiv * 8, top);
    
    bool dualChannel = m_pConfigDlg && m_pConfigDlg->isDualChannel();
    
    std::vector<double> incomingCH1 = m_ringBuffer->getAndClear();
    std::vector<double> incomingCH2 = m_ringBufferCH2->getAndClear();

    if (m_isPaused) {
        ui->grafica->replot();
        return;
    }
    if (incomingCH1.empty()) {
        ui->grafica->replot();
        return;
    }
    
    int numSamples = incomingCH1.size() / 2;
    int numSamples2 = incomingCH2.size() / 2;
    m_totalSamplesReceived += numSamples;
    
    qint64 elapsedMs = m_sampleTimer.elapsed();
    
    // Update average sample rate every 500ms
    if (elapsedMs > 500) {
        if (m_totalSamplesReceived > 0) {
            m_measuredSampleRate = (m_totalSamplesReceived * 1000.0) / elapsedMs;
        }
        m_sampleTimer.restart();
        m_totalSamplesReceived = 0;
    }
    
    double sampleRate = m_measuredSampleRate; 
    
    // Fallback to baud rate calculation for the first 500ms or if no data
    if (sampleRate < 1.0) {
        sampleRate = 1000000.0 / 10.0; // Default
        if (m_pConfigDlg) {
            sampleRate = m_pConfigDlg->getBaud() / 10.0;
            if (dualChannel) sampleRate /= 2.0;
        }
    }
    const double TIME_STEP = 1.0 / sampleRate;

    if (m_dspPipeline) {
        m_dspPipeline->setSampleRate(sampleRate);
    }

    bool isFiltEnabled = ui->checkEnableMA->isChecked() || ui->checkEnableFIR->isChecked();
    ui->grafica->graph(2)->setVisible(isFiltEnabled);
    if (dualChannel) ui->grafica->graph(3)->setVisible(isFiltEnabled);

    QString trigModeStr = ui->comboTrigMode->currentText();
    bool isRolling = (trigModeStr == "Rolling");
    
    if (isRolling) {
        if (m_triggerState != ROLLING) {
            m_triggerState = ROLLING;
            ui->grafica->graph(0)->data()->clear();
            ui->grafica->graph(2)->data()->clear();
            if (dualChannel) {
                ui->grafica->graph(1)->data()->clear();
                ui->grafica->graph(3)->data()->clear();
            }
            m_currentTime = 0;
        }
        
        QVector<double> t_keys(numSamples);
        QVector<double> raw1(numSamples);
        QVector<double> filt1(numSamples);
        for (int i = 0; i < numSamples; ++i) {
            m_currentTime += TIME_STEP;
            t_keys[i] = m_currentTime;
            raw1[i] = incomingCH1[i * 2];
            filt1[i] = incomingCH1[i * 2 + 1];
        }
        
        ui->grafica->graph(0)->addData(t_keys, raw1);
        if (isFiltEnabled) {
            ui->grafica->graph(2)->addData(t_keys, filt1);
        }
        
        if (dualChannel && numSamples2 > 0) {
            QVector<double> t_keys2(numSamples2);
            QVector<double> raw2(numSamples2);
            QVector<double> filt2(numSamples2);
            double tempTime = m_currentTime - numSamples * TIME_STEP;
            for (int i = 0; i < numSamples2; ++i) {
                tempTime += TIME_STEP;
                t_keys2[i] = tempTime;
                raw2[i] = incomingCH2[i * 2];
                filt2[i] = incomingCH2[i * 2 + 1];
            }
            ui->grafica->graph(1)->addData(t_keys2, raw2);
            if (isFiltEnabled) {
                ui->grafica->graph(3)->addData(t_keys2, filt2);
            }
        } else if (!dualChannel) {
            ui->grafica->graph(1)->data()->clear();
            ui->grafica->graph(3)->data()->clear();
        }
        
        ui->grafica->graph(0)->data()->removeBefore(m_currentTime - windowSize * 1.5);
        ui->grafica->graph(2)->data()->removeBefore(m_currentTime - windowSize * 1.5);
        if (dualChannel) {
            ui->grafica->graph(1)->data()->removeBefore(m_currentTime - windowSize * 1.5);
            ui->grafica->graph(3)->data()->removeBefore(m_currentTime - windowSize * 1.5);
        }
        
        ui->grafica->xAxis->setRange(m_currentTime - windowSize, m_currentTime);
        ui->grafica->replot();
        updateMeasurements();
        
    } else {
        // Trigger Mode
        if (m_triggerState == ROLLING) {
            m_triggerState = WAITING;
            m_trigX1.clear(); m_trigY1.clear(); m_trigY1Filt.clear();
            m_trigX2.clear(); m_trigY2.clear(); m_trigY2Filt.clear();
            m_samplesWaited = 0;
        }
        
        m_samplesNeeded = (int)(windowSize / TIME_STEP);
        bool isAuto = (trigModeStr == "Auto");
        bool isRising = (ui->comboTrigEdge->currentText() == "Rising");
        double trigLevel = ui->spinTrigLevel->value();
        
        bool frameComplete = false;
        
        for (int i = 0; i < numSamples; ++i) {
            double valRaw = incomingCH1[i * 2];
            double valFilt = incomingCH1[i * 2 + 1];
            
            if (m_triggerState == WAITING) {
                bool triggered = false;
                
                if (isRising && m_lastTriggerVolt < trigLevel && valRaw >= trigLevel) {
                    triggered = true;
                } else if (!isRising && m_lastTriggerVolt > trigLevel && valRaw <= trigLevel) {
                    triggered = true;
                }
                
                if (!triggered && isAuto) {
                    m_samplesWaited++;
                    // 50 ms timeout for auto-trigger
                    if (m_samplesWaited > (sampleRate * 0.05)) {
                        triggered = true;
                    }
                }
                
                if (triggered) {
                    m_triggerState = COLLECTING;
                    m_trigX1.clear(); m_trigY1.clear(); m_trigY1Filt.clear();
                    m_trigX2.clear(); m_trigY2.clear(); m_trigY2Filt.clear();
                    m_samplesCollected = 0;
                }
            }
            
            if (m_triggerState == COLLECTING) {
                m_trigX1.append(m_samplesCollected * TIME_STEP);
                m_trigY1.append(valRaw);
                m_trigY1Filt.append(valFilt);

                if (dualChannel && i < numSamples2) {
                    m_trigX2.append(m_samplesCollected * TIME_STEP);
                    m_trigY2.append(incomingCH2[i * 2]);
                    m_trigY2Filt.append(incomingCH2[i * 2 + 1]);
                }
                
                m_samplesCollected++;
                if (m_samplesCollected >= m_samplesNeeded) {
                    frameComplete = true;
                    break;
                }
            }
            m_lastTriggerVolt = valRaw;
        }
        
        if (frameComplete) {
            ui->grafica->graph(0)->setData(m_trigX1, m_trigY1);
            if (isFiltEnabled) {
                ui->grafica->graph(2)->setData(m_trigX1, m_trigY1Filt);
            } else {
                ui->grafica->graph(2)->data()->clear();
            }

            if (dualChannel) {
                ui->grafica->graph(1)->setData(m_trigX2, m_trigY2);
                if (isFiltEnabled) {
                    ui->grafica->graph(3)->setData(m_trigX2, m_trigY2Filt);
                } else {
                    ui->grafica->graph(3)->data()->clear();
                }
            } else {
                ui->grafica->graph(1)->data()->clear();
                ui->grafica->graph(3)->data()->clear();
            }
            ui->grafica->xAxis->setRange(0, windowSize);
            ui->grafica->replot();
            
            updateMeasurements();
            
            m_triggerState = WAITING; // Ready for next frame
            m_samplesWaited = 0;
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
    
    for (auto i = it; i != end; ++i) {
        double v = i->value;
        if (v > max_v) max_v = v;
        if (v < min_v) min_v = v;
    }
    
    double mid = (max_v + min_v) / 2.0;
    int crossings = 0;
    double prev_v = it->value;
    
    double first_cross_time = -1.0;
    double last_cross_time = -1.0;
    
    for (auto i = it; i != end; ++i) {
        double v = i->value;
        double t = i->key;
        if ((prev_v <= mid && v > mid) || (prev_v >= mid && v < mid)) {
            crossings++;
            if (first_cross_time < 0) first_cross_time = t;
            last_cross_time = t;
        }
        prev_v = v;
    }
    
    ui->labVmax->setText(QString("Vmax: %1 V").arg(max_v, 0, 'f', 2));
    ui->labVmin->setText(QString("Vmin: %1 V").arg(min_v, 0, 'f', 2));
    ui->labVpp->setText(QString("Vpp: %1 V").arg(max_v - min_v, 0, 'f', 2));
    
    if (crossings >= 2 && last_cross_time > first_cross_time) {
        double actual_span = last_cross_time - first_cross_time;
        double half_period_avg = actual_span / (crossings - 1);
        double freq = 1.0 / (half_period_avg * 2.0);
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
        
        m_dspPipeline->setSampleRate(baud);
        m_dspPipeline->setVRef(m_pConfigDlg ? m_pConfigDlg->getVRef() : 5.0);
        m_dspPipeline->setDualChannel(m_pConfigDlg ? m_pConfigDlg->isDualChannel() : false);
        m_dspPipeline->start();

        m_renderTimer->start();
        m_currentTime=0.0;
        m_plotData.clear();
        m_ringBuffer->clear();
        m_ringBufferCH2->clear();
        
        m_triggerState = WAITING;

        m_sampleTimer.start();
        m_totalSamplesReceived = 0;
        m_measuredSampleRate = 0.0;

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
    if (m_dspPipeline) {
        m_dspPipeline->stop();
    }
    m_renderTimer->stop();
    m_isConnected = false;
    ui->btnConnect->setEnabled(true);
    ui->btnDisconnect->setEnabled(false);
    ui->btnSettings->setEnabled(true);
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

    if (!m_pCursorDlg) {
        m_pCursorDlg = new cursordata(this);
    }

    if (m_cursorIdx == 1 && !m_pCursorDlg->isVisible()) {
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

void MainWindow::on_dialTimeDiv_valueChanged(int value)
{
    QStringList timeOptions = {"1 us", "2 us", "5 us", "10 us", "20 us", "50 us", "100 us", "200 us", "500 us", "1 ms", "2 ms", "5 ms", "10 ms", "20 ms", "50 ms", "100 ms", "200 ms", "500 ms", "1 s"};
    if (value >= 0 && value < timeOptions.size()) {
        ui->label_tdiv->setText("Time: " + timeOptions[value]);
    }
}

void MainWindow::on_dialVoltsDiv_valueChanged(int value)
{
    QStringList voltsOptions = {"0.1 V", "0.2 V", "0.5 V", "1 V", "2 V"};
    if (value >= 0 && value < voltsOptions.size()) {
        ui->label_vdiv->setText("Volts: " + voltsOptions[value]);
    }
}

void MainWindow::on_dialYOffset_valueChanged(int value)
{
    double offset = value * 0.1;
    ui->label_offset->setText(QString("Offset: %1 V").arg(offset, 0, 'f', 2));
}

void MainWindow::on_checkEnableMA_stateChanged(int arg1)
{
    if (m_dspPipeline) {
        m_dspPipeline->setEnableMovingAverage(arg1 != 0, ui->spinMASize->value());
    }
}

void MainWindow::on_spinMASize_valueChanged(int arg1)
{
    if (m_dspPipeline && ui->checkEnableMA->isChecked()) {
        m_dspPipeline->setEnableMovingAverage(true, arg1);
    }
}

void MainWindow::on_checkEnableFIR_stateChanged(int arg1)
{
    if (m_dspPipeline) {
        m_dspPipeline->setEnableFIR(arg1 != 0);
    }
}

void MainWindow::on_checkEnableFFT_stateChanged(int arg1)
{
    if (m_dspPipeline) {
        m_dspPipeline->setEnableFFT(arg1 != 0);
    }
    if (arg1 == 0) {
        m_fftPlot->graph(0)->data()->clear();
        if (m_fftPlot->graphCount() > 1) {
            m_fftPlot->graph(1)->data()->clear();
        }
        m_fftPlot->replot();
        m_fftWindow->hide();
    } else {
        m_fftWindow->show();
    }
}

void MainWindow::on_comboWindowType_currentIndexChanged(int index)
{
    if (m_dspPipeline) {
        m_dspPipeline->setWindowType(static_cast<DSPPipeline::WindowType>(index));
    }
}

void MainWindow::onFftDataReady(QVector<double> freqs, QVector<double> magCH1, QVector<double> magCH2)
{
    if (!m_fftWindow->isVisible()) return; // Only draw if visible

    if (!magCH1.isEmpty()) {
        m_fftPlot->graph(0)->setData(freqs, magCH1);
    }
    if (!magCH2.isEmpty() && m_fftPlot->graphCount() > 1) {
        m_fftPlot->graph(1)->setData(freqs, magCH2);
    }

    if (!freqs.isEmpty()) {
        m_fftPlot->xAxis->setRange(0, std::min(freqs.last(), 100000.0));
    }
    
    m_fftPlot->replot();
}

