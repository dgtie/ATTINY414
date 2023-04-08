BIN = $(HOME)/.platformio/packages/toolchain-atmelavr/bin
GPP = $(BIN)/avr-g++
OBJCOPY = $(BIN)/avr-objcopy
SIZE = $(BIN)/avr-size
PYMCUPROG = pymcuprog

SRC = $(shell ls -r *.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = out
MCU = attiny414

CFLAGS = -mmcu=$(MCU)
LDFLAGS = -mmcu=$(MCU) -Wl,-Map=$(TARGET).map

all: $(TARGET).hex

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

$(TARGET).elf: $(OBJ)
	$(GPP) $(LDFLAGS) -o $@ $^
	$(SIZE) $@

%.o: %.cpp
	$(GPP) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o $(TARGET).* app.*

flash:
	../bootapp/prog app.hex

updi: $(TARGET).hex
	pymcuprog write --erase -f $< -d attiny414 -t uart -u /dev/ttyACM0
