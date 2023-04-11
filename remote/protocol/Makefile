BIN = $(HOME)/.platformio/packages/toolchain-atmelavr/bin
GPP = $(BIN)/avr-g++
OBJCOPY = $(BIN)/avr-objcopy
SIZE = $(BIN)/avr-size

SRC = $(shell ls -r *.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = out
MCU = attiny414

CFLAGS = -mmcu=$(MCU) -Os
LDFLAGS = -mmcu=$(MCU) -Wl,-Map=$(TARGET).map -nostartfiles \
          -Wl,--section-start=.text=0xa00

all: $(TARGET).hex

%.hex: %.elf
	cp $< a.elf
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

%.elf: $(OBJ)
	$(GPP) $(LDFLAGS) -o $@ $^
	$(SIZE) $@

%.o: %.cpp
	$(GPP) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o $(TARGET).*

flash: $(TARGET).hex
	../../bootapp/prog $<

dasm:
	$(HOME)/.platformio/packages/toolchain-atmelavr/bin/avr-objdump -S a.elf > dasm
