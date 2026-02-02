#include "mainwindow.h"
#include "ui_MainWindow.h" // Make sure your .ui file is renamed or this matches
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
    // 100,000 points (~10 seconds @ 9.6kHz)
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
/*
void MainWindow::updatePlot()
{
    std::vector<double> yRaw = m_ringBuffer->getLast(2000);
    m_ringBuffer->clear();

    if (yRaw.empty()) return;

    if (yRaw.size() > 4000) {
        std::vector<double> yRecent(yRaw.end() - 4000, yRaw.end());
        yRaw = yRecent;
    }

    qDebug()<< "cant puntos: "<<yRaw.size();
    std::reverse(yRaw.begin(),yRaw.end());
    QVector<double> yVec(yRaw.begin(), yRaw.end());

    if (!yVec.isEmpty()) {
        QString debugStr = "First 5 Volts: ";
        for(int i = 0; i < std::min(5, (int)yVec.size()); ++i) {
            debugStr += QString::number(yVec[i]) + ", ";
        }
        qDebug() << debugStr;
    }

    QVector<double> xVec(yVec.size());

    // 500k / 13 cycles = ~38461 Hz Sample Rate
    double timeStep = 1.0 / 38461.0; // ~26 microseconds

    for(int i=0;i<yVec.size();i++){
		xVec[i]= m_currentTime+(i*timeStep);
	}
	m_currentTime=xVec.last();
	
    ui->grafica->graph(0)->setData(xVec, yVec);
    ui->grafica->xAxis->setRange(m_currentTime-2, m_currentTime);
    ui->grafica->graph(0)->data()->removeBefore(m_currentTime - 2 - 1.0);
    ui->grafica->replot();
}
*/

void MainWindow::updatePlot()
{
    // 1. Fetch ALL waiting data
    std::vector<double> incomingRaw = m_ringBuffer->getAll();
    m_ringBuffer->clear();

    if (incomingRaw.empty()) return;

    // --- SAFETY VALVE: Prevent "Insanely Fast" Scroll ---
    // If we have too much data (e.g., >2000 points), the screen would jump
    // too fast. We throw away the old history and just show the newest bit.
    if (incomingRaw.size() >250) {
        std::vector<double> recent(incomingRaw.end() - 250, incomingRaw.end());
        incomingRaw = recent;
    }

    // 2. Prepare Data Vectors
    QVector<double> yVec;
    QVector<double> xVec;

    // Reserve memory to prevent reallocation (Faster)
    yVec.reserve(incomingRaw.size());
    xVec.reserve(incomingRaw.size());

    // 3. Timebase Calculation
    // Bare Metal 38kHz: 26 microseconds per sample
    const double TIME_STEP = 0.000026*2;

    // 4. Manual Loop (Fixes "fromStdVector" error)
    for (double val : incomingRaw) {
        yVec.append(val);

        // Advance time for each individual point
        m_currentTime += TIME_STEP;
        xVec.append(m_currentTime);
    }

    // 5. Append to Graph (Rolling Mode)
    // This adds the new trail to the existing line
    ui->grafica->graph(0)->addData(xVec, yVec);

    // 6. Scroll the View
    // Make the X-Axis follow the latest time (Showing last 2 seconds)
    double windowSize = 2.0;
    ui->grafica->xAxis->setRange(m_currentTime - windowSize, m_currentTime);

    // 7. Cleanup Memory
    // removeBefore() deletes old data so the app doesn't run out of RAM
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
/*
    if(m_pConfigDlg){
        baud=m_pConfigDlg->getBaud();
    }
*/
    m_serialHandler->connectPort(port->portName(), baud);

    if (port->isOpen()) {
        m_isConnected = true;
        m_renderTimer->start();
        m_currentTime=0.0;
        m_plotData.clear(); // Force the buffer to rebuild cleanly
        m_ringBuffer->clear();

        // Update Buttons
        ui->btnConnect->setEnabled(false);
        ui->btnDisconnect->setEnabled(true);
        ui->btnSettings->setEnabled(false); // Lock settings while connected

        // Update Label
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
			ui->grafica->replot();
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

    for(int i=0; i<qtTime.size(); ++i) {
        qtTime[i] = i * 0.000013; // 13 microseconds per sample
    }

    m_pExportDlg = new exportdlg(this,ui->grafica,&qtTime,&qtVolts,m_pGraphOptDlg);
    m_pExportDlg->exec();
}

void MainWindow::onGraphClicked(QMouseEvent *event)
{
    // 1. Safety Checks
    if (!ui->checkCursors->isChecked()) return;
    if (ui->grafica->graphCount() == 0) return; // Prevent crash if no graph exists

    double xVal = ui->grafica->xAxis->pixelToCoord(event->pos().x());

    // 2. Move Cursor Line (Suspect #1: Ensure Constructor code above is added!)
    if (m_cursors[m_cursorIdx]) {
        m_cursors[m_cursorIdx]->point1->setCoords(xVal, 0);
        m_cursors[m_cursorIdx]->point2->setCoords(xVal, 5);
        m_cursors[m_cursorIdx]->setVisible(true);
    }

    // 3. Move Label
    double yTop = ui->grafica->yAxis->range().upper;
    if (m_cursorLabels[m_cursorIdx]) {
        m_cursorLabels[m_cursorIdx]->setText(QString::number(m_cursorIdx+1));
        m_cursorLabels[m_cursorIdx]->position->setCoords(xVal +  0.1, yTop * 0.8);
        m_cursorLabels[m_cursorIdx]->setVisible(true);
    }

    // 4. Get Data Value
    double voltage = 0.0;
    auto dataContainer = ui->grafica->graph(0)->data();
    auto it = dataContainer->findBegin(xVal);
    if(it!= dataContainer->end()) voltage=it->value;

    if (m_cursorIdx == 1) {
        m_pCursorDlg = new cursordata(this);
        // [FIX] Check if dialog exists. If not, create it.
        if (!m_pCursorDlg) m_pCursorDlg->show();

        // Ensure the dialog is visible
        if (!m_pCursorDlg->isVisible()) m_pCursorDlg->show();

        // Pass the values
        m_pCursorDlg->updateValue(xVal, voltage);
    }

    // 6. Cycle Index
    m_cursorIdx = (m_cursorIdx + 1) % 2;
    ui->grafica->replot();
}

/*
void MainWindow::onGraphClicked(QMouseEvent *event)
    {
        // 1. Safety Check
        if (!ui->checkCursors->isChecked()) return;
        if (ui->grafica->graphCount() == 0) return;

        double xVal = ui->grafica->xAxis->pixelToCoord(event->pos().x());

        m_cursors[m_cursorIdx]->point1->setCoords(xVal, 0);
        m_cursors[m_cursorIdx]->point2->setCoords(xVal, 5); // Just needs to be different Y
        m_cursors[m_cursorIdx]->setVisible(true);

        double yTop = ui->grafica->yAxis->range().upper;
        m_cursorLabels[m_cursorIdx]->position->setCoords(xVal + (xVal * 0.01), yTop * 0.8);
        m_cursorLabels[m_cursorIdx]->setVisible(true);

        double voltage = 0.0;
        auto dataContainer = ui->grafica->graph(0)->data();
        auto it = dataContainer->findBegin(xVal);
        if (it != dataContainer->end()) {
            voltage = it->value;
        }

        qDebug() << "Cursor" << m_cursorIdx + 1 << ": Time=" << xVal << "s, Volt=" << voltage << "V";

        if (m_cursorIdx == 1) {
            m_pCursorDlg->show();
            m_pCursorDlg->updateValue(xVal,voltage);
        }

        m_cursorIdx = (m_cursorIdx + 1) % 2;

        ui->grafica->replot();
    }
*/
/*

void MainWindow::onGraphClicked(QMouseEvent *event)
{
    if (!ui->checkCursors->isChecked()) return;

    double xCoord = ui->grafica->xAxis->pixelToCoord(event->pos().x());
    double yCoord = ui->grafica->xAxis->pixelToCoord(event->pos().y());

    // Vertical line: X is constant, Y spans bottom to top
    v_cursor->start->setCoords(xCoord, -10);
    v_cursor->end->setCoords(xCoord, 10);
    v_cursor->setVisible(true);

    // Horizontal line: Y is constant, X spans left to right
    h_cursor->start->setCoords(-10000, yCoord);
    h_cursor->end->setCoords(10000, yCoord);
    h_cursor->setVisible(true);

    if (!m_pCursorDlg) {
        m_pCursorDlg = new cursordata(this);
    }

    m_pCursorDlg->updateValue(xCoord, yCoord);

    m_pCursorDlg->show();


    ui->grafica->replot();
}
*/






void MainWindow::on_checkCursors_checkStateChanged(const Qt::CheckState &arg1)
{
    if(ui->checkCursors->checkState()==0){
        v_cursor->setVisible(false);
        h_cursor->setVisible(false);
        for (auto a : m_cursors) a->setVisible(false);
    }
}

