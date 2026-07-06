#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QSerialPort>
#include "portconfig.h"
#include "graphoptions.h"
#include "cursordata.h"
#include "exportdlg.h"
#include "ringbuffer.h"
#include "rawringbuffer.h"
#include "SerialHandler.h"
#include "dsppipeline.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnConnect_clicked();
    void on_btnDisconnect_clicked();
    void on_btnSettings_clicked();      // Opens portconfig
    void on_btnGraphOptions_clicked();  // Opens graphoptions
    void on_btnExport_clicked();        // Opens exportdlg
    void on_btnPause_clicked();
    void applyGraphSettings();
    void updateMeasurements();
    void onGraphClicked(QMouseEvent *event); // for cursors
    void updatePlot();
    void on_checkCursors_checkStateChanged(const Qt::CheckState &arg1);

    void on_dialTimeDiv_valueChanged(int value);
    void on_dialVoltsDiv_valueChanged(int value);
    void on_dialYOffset_valueChanged(int value);

    // DSP Slots
    void on_checkEnableMA_stateChanged(int arg1);
    void on_spinMASize_valueChanged(int arg1);
    void on_checkEnableFIR_stateChanged(int arg1);
    void on_checkEnableFFT_stateChanged(int arg1);
    void on_comboWindowType_currentIndexChanged(int index);
    void onFftDataReady(QVector<double> freqs, QVector<double> magCH1, QVector<double> magCH2);

private:
    Ui::MainWindow *ui;

    rawringbuffer* m_rawBuffer;
    rawringbuffer* m_rawBufferCH2;
    ringbuffer* m_ringBuffer;
    ringbuffer* m_ringBufferCH2;
    ringbuffer* m_ringBufferFilt;
    ringbuffer* m_ringBufferCH2Filt;
    serialhandler* m_serialHandler;
    DSPPipeline* m_dspPipeline;
    QTimer* m_renderTimer;

    portconfig* m_pConfigDlg=nullptr;
    graphoptions* m_pGraphOptDlg=nullptr;

    QMainWindow* m_fftWindow;
    QCustomPlot* m_fftPlot;

    cursordata* m_pCursorDlg=nullptr;

    QVector<double> m_plotData;
    double m_currentTime;
    bool m_isConnected;
    bool m_isPaused = false;

    enum TriggerState { WAITING, COLLECTING, ROLLING };
    TriggerState m_triggerState = ROLLING;
    double m_lastTriggerVolt = 0.0;
    
    QVector<double> m_trigX1;
    QVector<double> m_trigY1;
    QVector<double> m_trigX2;
    QVector<double> m_trigY2;
    QVector<double> m_trigY1Filt;
    QVector<double> m_trigY2Filt;
    int m_samplesCollected = 0;
    int m_samplesNeeded = 0;
    int m_samplesWaited = 0;

    QElapsedTimer m_sampleTimer;
    long long m_totalSamplesReceived = 0;
    double m_measuredSampleRate = 0.0;


    QCPItemStraightLine* m_cursors[2];
    QCPItemText* m_cursorLabels[2];
    int m_cursorIdx=0;

    QCPItemLine* v_cursor;
    QCPItemLine* h_cursor;
};

#endif // MAINWINDOW_H
