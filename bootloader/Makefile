BIN = $(HOME)/.platformio/packages/toolchain-atmelavr/bin
GCC = $(BIN)/avr-gcc
OBJCOPY = $(BIN)/avr-objcopy
SIZE = $(BIN)/avr-size
PYMCUPROG = pymcuprog

SRC = $(shell ls -r *.c)
OBJ = $(SRC:.c=.o)
TARGET = out
MCU = attiny414

CFLAGS = -mmcu=$(MCU) -Os
LDFLAGS = -mmcu=$(MCU) -Wl,-Map=$(TARGET).map -nostartfiles

all: $(TARGET).hex

%.hex: %.elf
	cp $< a.elf
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

%.elf: $(OBJ)
	$(GCC) $(LDFLAGS) -o $@ $^
	$(SIZE) $@

%.o: %.c
	$(GCC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o $(TARGET).*

flash: $(TARGET).hex
	$(PYMCUPROG) erase -d $(MCU) -t uart -u /dev/ttyACM0 
	$(PYMCUPROG) write -f $(TARGET).hex -d $(MCU) -t uart -u /dev/ttyACM0

# write --erase
# $pymcuprog -t uart -u /dev/ttyACM0 -d attiny414 write -m fuses -o 8 -l 2
