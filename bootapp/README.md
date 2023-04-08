# Uploader Application for the UART Bootloader
The program assumes a USB-to-UART bridge is connected at /dev/ttyACM0.

It reads in a hex file and sends out the starting address first. Then follows the the data. After the last row of data is sent, it sends out a blank row (ie. all 0xff) to indicate end of data.

