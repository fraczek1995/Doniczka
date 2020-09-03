###############################################################################
# Makefile for the project Doniczka_v1
###############################################################################

## General Flags
PROJECT = Doniczka_v1
MCU = atmega32
TARGET = Doniczka_v1.elf
CC = avr-gcc

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=Doniczka_v1.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = main.o ADC.o DHT11.o RTC.o TWI.o Usart.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) Doniczka_v1.hex Doniczka_v1.eep Doniczka_v1.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ADC.o: ../ADC.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

DHT11.o: ../DHT11.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

RTC.o: ../RTC.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

TWI.o: ../TWI.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Usart.o: ../Usart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) Doniczka_v1.elf dep/* Doniczka_v1.hex Doniczka_v1.eep Doniczka_v1.lss Doniczka_v1.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

