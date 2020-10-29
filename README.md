# Floating-point-display-for-UNO
The display will consist of two 4 digit 7 segment led displays.
For maximum brightness each segment will be individually controlled using a pulsed signal.
Brighness will be controlled by increasing the mark space ratio of this signal.
The mark space ratio will be controlled using a potentiometer on one of the analog tp digital ports.

The 8 digits require 8 x 7 IO pins plus 7 more for the decimal point making a total of 63.

The ATtiny861 has 16 IO pins, one of which will be reserved for control via the USI UART DI pin.

4 devices provide 60 IO pins.

An additional ATtiny861 will communicate with the UNO via its USI operating as a master I2C.
Using the facility to switch USI ports it will also provide control for the remaining devices
usiing its USI UART DO pin.

