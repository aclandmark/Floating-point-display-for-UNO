# Floating-point-display-for-UNO
A new floating point display for the UNO is being developed.  See the pdf file for a system diagram and photograph.  
Two ATtiny devices are used to drive the display and the I2C bus is used to connect them to the UNO.

Whenever the UNO has new data to display it connects itself to the TWI/I2C bus.  The master interface which polls the UNO every mS:
	Takes the data
	Converts it to a string if necessary
	Displays the lower four digits
	Forwards the upper digits to the other device 

Four data types are recognised:
	Floating point strings			Integer strings
	Floating point data			Integer data

When the master device  detects a string it converts it to a numbers and returns the result to the UNO.  When it detects a number it simply displays it.

See the pdf file for a photo plus system diagram.

The AVR TWI is based on the Universal Serial Interface.  Code for the USI was taken from the following sources:
For the master device
http://edge.rit.edu/content/P14254/public/Design/Electronics/Controller/Code/Atmel%20Application%20Notes
Click on	I2C	AVR310	GCC	AVR310 Using the USI module as a TWI master
and for the slave device:
https://www.microchip.com/wwwAppNotes/AppNotes.aspx?appnote=en591197

In the absense of space for the full floating point library a floating point to askii subroutine was found at 
https://www.geeksforgeeks.org/convert-floating-point-number-string/
