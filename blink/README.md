# Sample User Application
To work with the bootloader, the fuse BOOTEND need to be set to 2.

```$pymcuprog -t uart -u /dev/ttyACM0 -d attiny414 write -m fuses -o 8 -l 2```

After changing BOOTEND, it is still possible to upload program using UPDI.

|upload method|BOOTEND|application start adress|IVSEL|
|-|-|-|-|
|UPDI|0|0x0000|clear|
|UPDI|2|0x0000|set|
|bootloader|2|0x0200|clear|

I prepared two Makefiles for BOOTEND = 2.

|Makefile name|upload|command line|
|-|-|-|
|Makefile|UPDI|$make|
|app|bootloader|$make -f app|


# My Program Framework
Please refer to [it](https://github.com/dgtie/STICK250/blob/main/blink/README.md "a simple framework").
