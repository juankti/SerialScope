#include "dsppipeline.h"
#include "pocketfft_hdronly.h"
#include <cmath>
#include <algorithm>
#include <complex>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 31-tap Low-pass FIR Filter coefficients scaled by 1024
static const int32_t FIR_COEFFS[31] = {
    -2, -4, -6, -7, -5, 1, 12, 28, 48, 69, 
    89, 107, 120, 128, 131, 132, 131, 128, 120, 107, 
    89, 69, 48, 28, 12, 1, -5, -7, -6, -4, -2
};
static const int FIR_TAPS = 31;
static const int FIR_DELAY = 15;

DSPPipeline::DSPPipeline(rawringbuffer* rawCH1, rawringbuffer* rawCH2, 
                         ringbuffer* outCH1, ringbuffer* outCH2, 
                         QObject* parent)
    : QThread(parent), m_rawCH1(rawCH1), m_rawCH2(rawCH2),
      m_outCH1(outCH1), m_outCH2(outCH2),
      m_running(false), m_dualChannel(false), m_sampleRate(100000.0), m_vref(5.0),
      m_enableMA(false), m_maWindowSize(16),
      m_enableFIR(false), m_enableFFT(false), m_windowType(WindowType::Hanning)
{
}

DSPPipeline::~DSPPipeline() {
    stop();
}

void DSPPipeline::stop() {
    m_running = false;
    wait();
}

void DSPPipeline::setSampleRate(double rate) {
    QMutexLocker locker(&m_mutex);
    m_sampleRate = rate;
}

void DSPPipeline::setVRef(double vref) {
    QMutexLocker locker(&m_mutex);
    m_vref = vref;
}

void DSPPipeline::setDualChannel(bool dual) {
    QMutexLocker locker(&m_mutex);
    m_dualChannel = dual;
}

void DSPPipeline::setEnableMovingAverage(bool enable, int windowSize) {
    QMutexLocker locker(&m_mutex);
    m_enableMA = enable;
    m_maWindowSize = windowSize;
}

void DSPPipeline::setEnableFIR(bool enable) {
    QMutexLocker locker(&m_mutex);
    m_enableFIR = enable;
}

void DSPPipeline::setEnableFFT(bool enable) {
    QMutexLocker locker(&m_mutex);
    m_enableFFT = enable;
}

void DSPPipeline::run() {
    m_running = true;
    m_fftTimer.start();
    while (m_running) {
        std::vector<uint8_t> dataCH1 = m_rawCH1->getAndClear();
        std::vector<uint8_t> dataCH2;
        if (m_dualChannel) {
            dataCH2 = m_rawCH2->getAndClear();
        }

        double vref, sampleRate;
        bool dual;
        {
            QMutexLocker locker(&m_mutex);
            vref = m_vref;
            sampleRate = m_sampleRate;
            dual = m_dualChannel;
        }

        if (!dataCH1.empty()) {
            processChannel(dataCH1, m_outCH1, false, vref, sampleRate);
        }
        if (dual && !dataCH2.empty()) {
            processChannel(dataCH2, m_outCH2, true, vref, sampleRate);
        }

        msleep(10); // Sleep for 10ms to yield
    }
}

void DSPPipeline::processChannel(const std::vector<uint8_t>& input, ringbuffer* outRaw, 
                                 bool isCH2, double vref, double sampleRate) 
{
    // Retrieve settings
    bool ma, fir, fft;
    int maSize;
    {
        QMutexLocker locker(&m_mutex);
        ma = m_enableMA;
        fir = m_enableFIR;
        fft = m_enableFFT;
        maSize = m_maWindowSize;
    }

    auto& history = isCH2 ? m_historyCH2 : m_historyCH1;
    
    // Append new data to history for overlap/filtering
    history.insert(history.end(), input.begin(), input.end());
    
    // Determine maximum delay needed
    int delay = 0;
    if (fir) delay = FIR_DELAY;
    else if (ma) delay = (maSize - 1) / 2;

    int historyNeeded = std::max(delay * 2 + 1, 8192); // Keep history for filters

    // Filtering
    std::vector<double> outRawData;
    outRawData.reserve(input.size());
    std::vector<double> outFiltData;
    outFiltData.reserve(input.size());

    // Filter processing
    for (size_t i = history.size() - input.size(); i < history.size(); ++i) {
        // Raw Data (delay compensated)
        if (i >= (size_t)delay) {
            outRawData.push_back((history[i - delay] / 255.0) * vref);
        } else {
            outRawData.push_back((history[i] / 255.0) * vref); // fallback
        }

        // Filtered Data
        if (fir && i >= (size_t)FIR_TAPS) {
            int32_t acc = 0;
            for (int k = 0; k < FIR_TAPS; ++k) {
                acc += history[i - k] * FIR_COEFFS[k];
            }
            uint8_t val = (uint8_t)std::clamp(acc >> 10, 0, 255);
            outFiltData.push_back((val / 255.0) * vref);
        }
        else if (ma && i >= (size_t)maSize) {
            uint32_t acc = 0;
            for (int k = 0; k < maSize; ++k) {
                acc += history[i - k];
            }
            uint8_t val = (uint8_t)(acc / maSize);
            outFiltData.push_back((val / 255.0) * vref);
        }
        else {
            outFiltData.push_back(outRawData.back());
        }
    }

    // Interleave Raw and Filt data for the GUI to guarantee perfect sync
    std::vector<double> combinedData(outRawData.size() * 2);
    for (size_t i = 0; i < outRawData.size(); ++i) {
        combinedData[i * 2] = outRawData[i];
        combinedData[i * 2 + 1] = outFiltData[i];
    }
    outRaw->push(combinedData.data(), combinedData.size());

    // FFT Processing
    if (fft) {
        // FFT is computed on the filtered data if filtering is active, else raw.
        // Since outFiltData is equal to outRawData when filters are off, we just use outFiltData.
        computeFFT(outFiltData, isCH2, sampleRate);
    }

    // Trim history to prevent memory leak
    if (history.size() > (size_t)historyNeeded) {
        history.erase(history.begin(), history.end() - historyNeeded);
    }
}

void DSPPipeline::setWindowType(WindowType type) {
    QMutexLocker locker(&m_mutex);
    m_windowType = type;
}

void DSPPipeline::computeFFT(const std::vector<double>& input, bool isCH2, double sampleRate) {
    auto& fftBuf = isCH2 ? m_fftBufferCH2 : m_fftBufferCH1;
    fftBuf.insert(fftBuf.end(), input.begin(), input.end());

    const size_t N = 4096;
    
    // Prevent memory bloat if we can't process fast enough
    if (fftBuf.size() > N * 2) {
        fftBuf.erase(fftBuf.begin(), fftBuf.end() - N);
    }

    if (fftBuf.size() >= N) {
        // Limit FFT rate to ~30 FPS (33ms)
        if (m_fftTimer.isValid() && m_fftTimer.elapsed() < 33) {
            return; 
        }
        m_fftTimer.restart();

        std::vector<double> rdata(N);
        
        // Window selection and DC removal
        double sum = 0;
        for (size_t i = 0; i < N; ++i) sum += fftBuf[i];
        double mean = sum / N;

        WindowType winType;
        {
            QMutexLocker locker(&m_mutex);
            winType = m_windowType;
        }

        for (size_t i = 0; i < N; ++i) {
            double window = 1.0;
            if (winType == WindowType::Hanning) {
                window = 0.5 * (1.0 - std::cos(2.0 * M_PI * i / (N - 1)));
            } else if (winType == WindowType::Hamming) {
                window = 0.54 - 0.46 * std::cos(2.0 * M_PI * i / (N - 1));
            } else if (winType == WindowType::Blackman) {
                window = 0.42 - 0.5 * std::cos(2.0 * M_PI * i / (N - 1)) + 0.08 * std::cos(4.0 * M_PI * i / (N - 1));
            }
            rdata[i] = (fftBuf[i] - mean) * window;
        }

        // PocketFFT
        pocketfft::shape_t shape = { N };
        pocketfft::stride_t stride_in = { sizeof(double) };
        pocketfft::stride_t stride_out = { 2 * sizeof(double) }; 
        
        std::vector<std::complex<double>> cdata(N / 2 + 1);
        
        pocketfft::shape_t axes = { 0 };
        
        pocketfft::r2c(shape, stride_in, stride_out, axes, pocketfft::FORWARD, rdata.data(), cdata.data(), 1.0);

        QVector<double> freqs;
        QVector<double> mags;
        
        // Skip Bin 0 (DC)
        for (size_t i = 1; i <= N / 2; ++i) { 
            double freq = (i * sampleRate) / N;
            double mag = std::abs(cdata[i]);
            double mag_db = 20.0 * std::log10(mag + 1e-6);
            
            freqs.append(freq);
            mags.append(mag_db);
        }

        if (isCH2) {
            emit fftDataReady(freqs, QVector<double>(), mags);
        } else {
            emit fftDataReady(freqs, mags, QVector<double>());
        }

        // 50% overlap for the next calculation
        fftBuf.erase(fftBuf.begin(), fftBuf.begin() + N / 2);
    }
}
