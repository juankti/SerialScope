8-bit serial data real-time plotter.

Born as a spin-off of [this project](), this software converts serial communication 8-bit raw binary data to a voltage value ranging from 0 to 5 V.

Initially designed for effectively seeing real-time analog sensors data using a ATmega328p ADC and its USART as a driver. While basic port settings are allowed, the software is best suited for a bare-metal application like [shown in this repository](), where the 1MHz-clocked ADC sends data through USART at 1Mbps.

Graph functions as clearing, grid, ranges, labels, clearing up to cursor measurements were developed.
Furthermore, the software allows exporting the graph as a image or as a .csv file, with various options included.