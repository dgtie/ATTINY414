#A UART Bootloader
- less than 512B
- BOOTEND = 2
- Application start =  0x200

At startup, it checks RA3 and data location 0x8200 (ie. 0x200 in program space) to determine if bootloader or user application shall be started.
The bootloader first waits for the start address. Then writes received data to flash. When a blank row of data (ie. all 0xff) is received, or it reaches end of flash, it reboots the system.

#Setting BOOTEND
```$pymcuprog -t uart -u /dev/ttyACM0 -d attiny414 write -m fuses -o 8 -l 2```

