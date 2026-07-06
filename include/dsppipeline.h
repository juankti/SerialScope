#ifndef DSPPIPELINE_H
#define DSPPIPELINE_H

#include <QThread>
#include <QVector>
#include <QMutex>
#include <QWaitCondition>
#include <QElapsedTimer>
#include "rawringbuffer.h"
#include "ringbuffer.h"

class DSPPipeline : public QThread {
    Q_OBJECT
public:
    enum class WindowType {
        Rectangular,
        Hanning,
        Hamming,
        Blackman
    };

    DSPPipeline(rawringbuffer* rawCH1, rawringbuffer* rawCH2, 
                ringbuffer* outCH1, ringbuffer* outCH2, 
                QObject* parent = nullptr);
    ~DSPPipeline();

    void stop();
    void setSampleRate(double rate);
    void setVRef(double vref);
    void setDualChannel(bool dual);

    void setEnableMovingAverage(bool enable, int windowSize = 16);
    void setEnableFIR(bool enable);
    void setEnableFFT(bool enable);
    void setWindowType(WindowType type);

signals:
    void fftDataReady(QVector<double> freqs, QVector<double> magCH1, QVector<double> magCH2);

protected:
    void run() override;

private:
    void processChannel(const std::vector<uint8_t>& input, ringbuffer* outRaw, 
                        bool isCH2, double vref, double sampleRate);
    void computeFFT(const std::vector<double>& input, bool isCH2, double sampleRate);

    rawringbuffer* m_rawCH1;
    rawringbuffer* m_rawCH2;
    ringbuffer* m_outCH1;
    ringbuffer* m_outCH2;

    bool m_running;
    bool m_dualChannel;
    double m_sampleRate;
    double m_vref;

    bool m_enableMA;
    int m_maWindowSize;
    bool m_enableFIR;
    bool m_enableFFT;
    WindowType m_windowType;

    QMutex m_mutex;

    // Filter states
    std::vector<uint8_t> m_historyCH1;
    std::vector<uint8_t> m_historyCH2;

    // FFT states
    std::vector<double> m_fftBufferCH1;
    std::vector<double> m_fftBufferCH2;
    QElapsedTimer m_fftTimer;
};

#endif // DSPPIPELINE_H
