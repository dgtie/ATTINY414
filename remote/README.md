#Programmable IR Remote Controller
- 12 keys (1 shift-key + 11 ordinary keys)
- 1 LED indicator
- 1 IR photo-transistor
- 2 IR LED
- UART
- UPDI
- support 12 devices
- each device has 22 keys (11 ordinary + 11 shifted)
- LEARN mode (read IR signal and send mark/space intervals to UART)
- programmable through UART (upgrade firmware / upload lookup table)
- SLEEP mode (sleeps when idle: LED off and not sending IR signals)

#Key index
|  IR LED | IR SENSOR  |  IR LED |
| :------------: | :------------: | :------------: |
|**(shift)**||LED|
|**11**| 0  | 10  |
| 1  | 2  | 3  |
|  4 |  5 | 6  |
| 7  | 8  |  9 |

#Modes and Operation

##CONTROL Mode
- click key#11 (shift-key) to toggle LED
- LED will turn off when no key is pressed in 5 seconds
- click other keys to send out IR signals. 22 signals supported.
- hold shift-key for 2 seconds to enter LEARN mode
- hold key#7 and click key#10 to enter BOOTLOADER mode.
- sleeps when idle

##LEARN Mode
- LED blinks slow
- won't sleep
- aim the remote to read close to the IR sensor (between the two IR LED)
- mark and space will be sent to UART. (reading times 24 us)
- zero space means time overflow
- click any keys to return to CONTROL Mode. key clicked determines the device to use.

##BOOTLOADER Mode
- LED blinks fast
- won't sleep

#Device and Key Table

|  0x8EC0 | #0  | #1  | #2  |  #3 | #4  | #5  |  #6 | #7  | #8  | #9  |#10|
| :------------: | :------------: | :------------: | :------------: | :------------: | :------------: | :------------: | :------------: | :------------: | :------------: | :------------: |:-:|
| EXTENDED  | device#0 or device#8  |  device#1 or device#9  |  device#2 or device#10  | device#3 or device#11   | device#4  | device#5 |  device#6 | device#7  |
| device#0  | ordinary  |||||||||||
|   | shifted   |   |   |   |   |   |   |   |   ||
|   | null-char | address | protocol low byte | protocol high byte |
| device#1 | ordinary  |   |   |   |   |   |   |   |   |   ||

- Table starts at 0x8EC0
- null-char is normally 0xff

