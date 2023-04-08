BIN = $(HOME)/.platformio/packages/toolchain-atmelavr/bin
GCC = $(BIN)/avr-gcc
OBJCOPY = $(BIN)/avr-objcopy
SIZE = $(BIN)/avr-size

SRC = $(shell ls -r *.c)
OBJ = $(SRC:.c=.o)
TARGET = out
MCU = attiny414

CFLAGS = -mmcu=$(MCU)
LDFLAGS = -mmcu=$(MCU) -Wl,-Map=$(TARGET).map -nostartfiles \
          -Wl,--section-start=.text=0xec0

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
	../../bootapp/prog $<

