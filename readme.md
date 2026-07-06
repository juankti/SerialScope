# SerialScope

A real-time software oscilloscope for microcontroller diagnostics, built within **Qt Framework**. 

<img width="800" height="450" alt="serialscope" src="https://github.com/user-attachments/assets/4b535e65-8c6f-4a97-a265-3d4e248bd083" />

Designed to overcome the latency and overhead limitations of standard serial monitors, SerialScope abandons ASCII string transmission in favor of **raw binary data streaming**. This allows the software to process and plot data streams at extreme speeds (up to 1,000,000+ baud) without dropping samples or freezing the UI, making it ideal for visualizing high-frequency ADC captures.

It features a robust, custom-built DSP engine. Because the software is designed to handle extremely high data throughput from MCUs (up to 1 MB/s over USB Virtual COM Ports), processing signals in the main GUI thread would cause severe bottlenecks. To solve this, the DSP architecture was built from the ground up prioritizing multithreading and memory efficiency.

##  Key Features

*   **Hardware-like Oscilloscope Toolkit:** 
    *   **Triggering System:** Stabilize periodic signals using Rising Edge, Falling Edge, and custom Voltage Level triggers. Features a dead-time simulated holdoff to prevent rendering saturation.
    *   **Rolling Mode:** Continuous, unbuffered data scrolling for live monitoring.
    *   **Dynamic Scaling:** Adjustable Time/Div (from 1 s down to 100 µs resolution) and Volts/Div with customizable Y-Offset and V_ref anchoring.
*   **FFT Spectrum Analyzer:** A fully interactive Frequency Domain viewer powered by the highly optimized `PocketFFT` library. It lives in a dynamic floating window that can be moved to a secondary monitor, allowing simultaneous Time-Domain and Frequency-Domain analysis. Features multiple windowing functions (Hanning, Hamming, Blackman, Rectangular).
*   **Real-Time Digital Filters**: dynamic Moving Average and FIR filters to clean up noisy signals.
*   **Real-Time Rendering Engine:** Engineered using `QCustomPlot` and a decoupled architecture that separates the heavy I/O serial polling from the GUI event loop, maintaining a fluid 60 FPS plotting experience even under massive data loads.
*   **Automated Signal Analytics:** On-the-fly math algorithms calculate $V_{max}$, $V_{min}$, Peak-to-Peak Voltage ($V_{pp}$), and perform highly accurate frequency estimation via precise zero-crossing timestamp tracking.
*   **Essential analytical tools** are built directly into the interface. Users can manipulate grid settings, adjust signal ranges, and perform precise signal analysis using dual cursors for real-time voltage and time measurements. For documentation and post-processing, the software includes robust export capabilities, allowing the graph to be saved as high-resolution images or raw CSV datasets.

##  Architecture & Technical Highlights

This project emphasizes robust memory management, multithreading, and optimization:

*   **Thread-Safe Ring Buffer:** Implements a custom, mutex-locked circular buffer tailored for high-frequency interrupts. It features an atomic `getAndClear()` routine to extract and reset memory blocks simultaneously, completely eliminating race conditions and data-drop during rapid thread context switching.
 **Interleaved Memory & Batch Rendering:** To achieve buttery-smooth 60 FPS rendering while plotting hundreds of thousands of data points, raw and filtered data are interleaved in memory (`[Raw, Filt, Raw, Filt]`). This guarantees perfect time-sync without complex mutex locks and allows injecting data into `QCustomPlot` through rapid vector batching rather than slow point-by-point loops.
*   **Decoupled I/O & UI:** The `SerialHandler` runs entirely asynchronous to the main UI. Data is safely pushed into the ring buffer via serial interrupts, while a dedicated `QTimer` acts as a rendering orchestrator, pulling chunks of data at fixed intervals. 
*   **Raw Binary Protocol:** By transmitting raw 8-bit values (`0x00` - `0xFF`) instead of formatted strings (e.g., `"1023\n"`), the system cuts communication overhead by more than 75%. The software dynamically scales these 8-bit values back to physical voltages based on user-defined reference parameters.

##  Microcontroller Integration

Instead of using string-based printing (like `Serial.print()`), the MCU must send the raw 8-bit ADC readings directly over the serial port. The baud rate must then be matched in the software settings and data will instantly appear on the grid.

### Acknowledgments & Third-Party Libraries
* **[PocketFFT](https://gitlab.mpcdf.mpg.de/mtr/pocketfft):** The Fast Fourier Transform calculations in this software are powered by `pocketfft` (C++ version), authored by Martin Reinecke. It was chosen for its outstanding performance and lightweight header-only implementation.
* **[QCustomPlot](https://www.qcustomplot.com/):** Used for the high-performance plotting and rendering of the oscilloscope graphs.

## Example
<img width="800" height="450" alt="rolling" src="https://github.com/user-attachments/assets/2b33c496-2fe3-4a8b-a60c-4291cc296ebc" />
