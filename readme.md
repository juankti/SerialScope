# SerialScope

A real-time software oscilloscope for microcontroller diagnostics, built within **Qt Framework**. 

Designed to overcome the latency and overhead limitations of standard serial monitors, SerialScope abandons ASCII string transmission in favor of **raw binary data streaming**. This allows the software to process and plot data streams at extreme speeds (up to 1,000,000+ baud) without dropping samples or freezing the UI, making it ideal for visualizing high-frequency ADC captures.

##  Key Features

*   **Real-Time Rendering Engine:** Engineered using `QCustomPlot` and a decoupled architecture that separates the heavy I/O serial polling from the GUI event loop, maintaining a fluid 60 FPS plotting experience even under massive data loads.
*   **Hardware-like Oscilloscope Toolkit:** 
    *   **Triggering System:** Stabilize periodic signals using Rising Edge, Falling Edge, and custom Voltage Level triggers. Features a dead-time simulated holdoff to prevent rendering saturation.
    *   **Rolling Mode:** Continuous, unbuffered data scrolling for live monitoring.
    *   **Dynamic Scaling:** Adjustable Time/Div (from 1 s down to 100 µs resolution) and Volts/Div with customizable Y-Offset and V_ref anchoring.
*   **Automated Signal Analytics:** On-the-fly math algorithms calculate $V_{max}$, $V_{min}$, Peak-to-Peak Voltage ($V_{pp}$), and perform highly accurate frequency estimation via precise zero-crossing timestamp tracking.
*   **Essential analytical tools** are built directly into the interface. Users can manipulate grid settings, adjust signal ranges, and perform precise signal analysis using dual cursors for real-time voltage and time measurements. For documentation and post-processing, the software includes robust export capabilities, allowing the graph to be saved as high-resolution images or raw CSV datasets.

##  Architecture & Technical Highlights

This project emphasizes robust memory management, multithreading, and optimization:

*   **Thread-Safe Ring Buffer:** Implements a custom, mutex-locked circular buffer tailored for high-frequency interrupts. It features an atomic `getAndClear()` routine to extract and reset memory blocks simultaneously, completely eliminating race conditions and data-drop during rapid thread context switching.
*   **Decoupled I/O & UI:** The `SerialHandler` runs entirely asynchronous to the main UI. Data is safely pushed into the ring buffer via serial interrupts, while a dedicated `QTimer` acts as a rendering orchestrator, pulling chunks of data at fixed intervals. 
*   **Raw Binary Protocol:** By transmitting raw 8-bit values (`0x00` - `0xFF`) instead of formatted strings (e.g., `"1023\n"`), the system cuts communication overhead by more than 75%. The software dynamically scales these 8-bit values back to physical voltages based on user-defined reference parameters.

##  Microcontroller Integration

Instead of using string-based printing (like `Serial.print()`), the MCU must send the raw 8-bit ADC readings directly over the serial port. The baud rate must then be matched in the software settings and data will instantly appear on the grid.
