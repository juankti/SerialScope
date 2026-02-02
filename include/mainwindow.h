#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSerialPort>
#include <algorithm>
#include "portconfig.h"
#include "graphoptions.h"
#include "cursordata.h"
#include "exportdlg.h"
#include "ringbuffer.h"
#include "serialhandler.h"

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
    void on_btnClear_clicked();

    void onGraphClicked(QMouseEvent *event); // for cursors
    void updatePlot();
    void on_checkCursors_checkStateChanged(const Qt::CheckState &arg1);

private:
    Ui::MainWindow *ui;

    ringbuffer* m_ringBuffer;
    serialhandler* m_serialHandler;
    QTimer* m_renderTimer;

    portconfig* m_pConfigDlg=nullptr;
    graphoptions* m_pGraphOptDlg=nullptr;
    exportdlg* m_pExportDlg=nullptr;
    cursordata* m_pCursorDlg=nullptr;

    QVector<double> m_plotData;
    double m_currentTime;
    bool m_isConnected;

    QCPItemStraightLine* m_cursors[2];
    QCPItemText* m_cursorLabels[2];
    int m_cursorIdx=0;

    QCPItemLine* v_cursor;
    QCPItemLine* h_cursor;
};

#endif // MAINWINDOW_H
