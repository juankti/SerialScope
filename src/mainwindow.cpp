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
    ui->grafica->setInteraction(QCP::iRangeDrag,true);
    ui->grafica->setInteraction(QCP::iRangeZoom,true);
    m_ringBuffer = new ringbuffer(100000);

    m_serialHandler = new serialhandler(m_ringBuffer, this);

    m_renderTimer = new QTimer(this);
    connect(m_renderTimer, &QTimer::timeout, this, &MainWindow::updatePlot);

    ui->grafica->addGraph();
    ui->grafica->graph(0)->setPen(QPen(Qt::green)); 
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
}

MainWindow::~MainWindow()
{
    delete m_serialHandler;
    delete m_ringBuffer;
    delete ui;
}


void MainWindow::updatePlot()
{
    std::vector<double> incomingRaw = m_ringBuffer->getAll();
    m_ringBuffer->clear();

    if (incomingRaw.empty()) return;

    if (incomingRaw.size() >250) {
        std::vector<double> recent(incomingRaw.end() - 250, incomingRaw.end());
        incomingRaw = recent;
    }

    QVector<double> yVec;
    QVector<double> xVec;
    yVec.reserve(incomingRaw.size());
    xVec.reserve(incomingRaw.size());

    float sampleRate = m_pConfigDlg->getBaud()/10;

    const double TIME_STEP = 12.0/sampleRate; // for matching with actual time

    for (double val : incomingRaw) {
        yVec.append(val);

        m_currentTime += TIME_STEP;
        xVec.append(m_currentTime);
    }

    ui->grafica->graph(0)->addData(xVec, yVec);

    double windowSize = 2.0;
    ui->grafica->xAxis->setRange(m_currentTime - windowSize, m_currentTime);

    ui->grafica->graph(0)->data()->removeBefore(m_currentTime - windowSize - 1.0);
    ui->grafica->replot();


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
    }

    m_serialHandler->connectPort(port->portName(), baud);

    if (port->isOpen()) {
        m_isConnected = true;
        m_renderTimer->start();
        m_currentTime=0.0;
        m_plotData.clear();
        m_ringBuffer->clear();


        ui->btnConnect->setEnabled(false);
        ui->btnDisconnect->setEnabled(true);
        ui->btnSettings->setEnabled(false);


        ui->labOpenPort->setText("Connected to " + port->portName() + " @ 9.6k");
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

void MainWindow::on_btnClear_clicked()
{
    m_ringBuffer->clear();
    m_currentTime = 0.0;
    ui->grafica->graph(0)->data()->clear();
    ui->grafica->replot();
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

    m_pExportDlg = new exportdlg(this,ui->grafica,&qtTime,&qtVolts,m_pGraphOptDlg);
    m_pExportDlg->exec();
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


void MainWindow::on_checkCursors_checkStateChanged(const Qt::CheckState &arg1)
{
    if(ui->checkCursors->checkState()==0){
        v_cursor->setVisible(false);
        h_cursor->setVisible(false);
        for (auto a : m_cursors) a->setVisible(false);
    }
}

