# Arduino Makefile
#
# This makefile allows you to build sketches from the command line
# without the Arduino environment (or Java).
#
# The Arduino environment does preliminary processing on a sketch before
# compiling it.  If you're using this makefile instead, you'll need to do
# a few things differently:
#
#   - Give your program's file a .cpp extension (e.g. foo.cpp).
#
#   - Put this line at top of your code: #include <Arduino.h>
#
#   - Write prototypes for all your functions (or define them before you
#     call them).  A prototype declares the types of parameters a
#     function will take and what type of value it will return.  This
#     means that you can have a call to a function before the definition
#     of the function.  A function prototype looks like the first line of
#     the function, with a semi-colon at the end.  For example:
#     int digitalRead(int pin);
#
# Instructions for using the makefile:
#
#  1. Copy this file into the folder with your sketch.
#
#  2. Below, modify the line containing "TARGET" to refer to the name of
#     of your program's file without an extension (e.g. TARGET = foo).
#
#  3. Modify the line containg "ARDUINO" to point the directory that
#     contains the Arduino core (for normal Arduino installations, this
#     is the hardware/cores/arduino sub-directory).
#
#  4. Modify the line containing "PORT" to refer to the filename
#     representing the USB or serial connection to your Arduino board
#     (e.g. PORT = /dev/tty.USB0).  If the exact name of this file
#     changes, you can use * as a wildcard (e.g. PORT = /dev/tty.USB*).
#
#  5. At the command line, change to the directory containing your
#     program's file and the makefile.
#
#  6. Type "make" and press enter to compile/verify your program.
#
#  7. Type "make upload", reset your Arduino board, and press enter to
#     upload your program to the Arduino board.
#
# $Id$

PORT = /dev/ttyUSB0
TARGET = display

ARDUINO = /home/mathieu/opt/arduino
ARDUINO_SRC = $(ARDUINO)/hardware/arduino/cores/arduino
ARDUINO_LIB_SRC = $(ARDUINO)/libraries


# Board (and processor) information: see $(ARDUINO_PATH)hardware/arduino/boards.txt
# Some examples:
#	BOARD		DESCRIPTION
#	uno			Arduino Uno
#	atmega328	Arduino Duemilanove or Nano w/ ATmega328
#	diecimila		Arduino Diecimila, Duemilanove, or Nano w/ ATmega168
#	mega		Arduino Mega
#	mini			Arduino Mini
#	lilypad328	LilyPad Arduino w/ ATmega328
BOARD = atmega328

#=== fetch parameter from boards.txt processor parameter ===
# the basic idea is to get most of the information from boards.txt

BOARDS_TXT = $(ARDUINO)/hardware/arduino/boards.txt

#MCU = $(shell sed -n -e "s/$(BOARD).build.mcu=\(.*\)/\1/p" $(BOARDS_TXT))
#F_CPU = $(shell sed -n -e "s/$(BOARD).build.f_cpu=\(.*\)/\1/p" $(BOARDS_TXT))
#BUILD_VARIANT = $(shell sed -n -e "s/$(BOARD).build.variant=\(.*\)/\1/p" $(BOARDS_TXT))
#AVRDUDE_UPLOAD_RATE = $(shell sed -n -e "s/$(BOARD).upload.speed=\(.*\)/\1/p" $(BOARDS_TXT))
MCU = $(shell grep $(BOARD).build.mcu $(BOARDS_TXT) | sed 's/.*=//')
F_CPU = $(shell grep $(BOARD).build.f_cpu $(BOARDS_TXT) | sed 's/.*=//')
BUILD_VARIANT = $(shell grep $(BOARD).build.variant $(BOARDS_TXT) | sed 's/.*=//')
AVRDUDE_UPLOAD_RATE = $(shell grep $(BOARD).upload.speed $(BOARDS_TXT) | sed 's/.*=//')
AVRDUDE_PROGRAMMER = stk500v1
FORMAT = ihex

# List of all libaries which should be included.
EXTRA_DIRS=$(ARDUINO)/libraries/LiquidCrystal

############################################################################
# Below here nothing should be changed...

AVR_TOOLS_PATH = /usr/bin

# Arduino & Wiring libraries
############################
SRC = $(wildcard $(ARDUINO_SRC)/*.c)

CXXSRC = applet/$(TARGET).cpp \
	$(wildcard $(ARDUINO_SRC)/*.cpp)

# Place -I options here
INCLUDE = -I$(ARDUINO_SRC) -I$(ARDUINO)/hardware/tools/avr/avr/include \
	-I$(ARDUINO)/hardware/arduino/variants/$(BUILD_VARIANT) \
	-I$(ARDUINO_LIB_SRC)

# External Arduino Libraries
############################
#~ SRC     += $(ARDUINO_LIB_SRC)/Wire/utility/twi.c
CXXSRC += $(ARDUINO_LIB_SRC)/LiquidCrystal/LiquidCrystal.cpp
INCLUDE += -I$(ARDUINO_LIB_SRC)/LiquidCrystal
#~ CXXSRC	+= $(ARDUINO_LIB_SRC)/Wire/Wire.cpp

# CRAL Libraries
################
#~ CRAL  	=  $(ARDUINO_LIB_SRC)/cral
#~ CXXSRC	+= $(CRAL)/SPrint/staticPrint.cpp
#~ CXXSRC	+= $(CRAL)/twiLCD/twiLCD.cpp
#~ CXXSRC	+= $(CRAL)/ds1337/ds1337.cpp
#~ CXXSRC	+= $(CRAL)/ds1803/ds1803.cpp
#~ CXXSRC	+= $(CRAL)/ds1624/ds1624.cpp

##################
# End of libraries


# Name of this Makefile (used for "make depend").
MAKEFILE = Makefile

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
DEBUG = stabs

#Faster
#OPT = 3
#OPT = 2
#OPT = s

#Smaller
OPT = s -mcall-prologues

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU)
CXXDEFS = -DF_CPU=$(F_CPU)

HEADERS = $(wildcard $(ARDUINO_SRC)/*.h) $(wildcard $(ARDUINO_LIB_SRC)/*/*.h)

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99
CDEBUG = -g$(DEBUG)
CWARN = -Wall -Wstrict-prototypes
CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)

CFLAGS = $(CDEBUG) $(CDEFS) $(INCLUDE) -O$(OPT) $(CWARN) $(CSTANDARD) $(CEXTRA)
CXXFLAGS = $(CDEFS) $(INCLUDE) -O$(OPT)
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs
LDFLAGS = -O$(OPT) -lm -Wl,--gc-sections

# Programming support using avrdude. Settings and variables.
AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -U flash:w:applet/$(TARGET).hex
AVRDUDE_FLAGS = -V -F -C /etc/avrdude.conf \
-p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) \
-b $(AVRDUDE_UPLOAD_RATE)

# Program settings
CC = avr-gcc
CXX = avr-g++
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AR  = avr-ar
SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
MV = mv -f

# Define all object files.
OBJ = $(SRC:.c=.o) $(CXXSRC:.cpp=.o) $(ASRC:.S=.o)

# Define all listing files.
LST = $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst) $(SRC:.c=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS)
ALL_CXXFLAGS = -mmcu=$(MCU) -I. $(CXXFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

# Default target.
all: applet_files build sizeafter

build: elf hex bin s

applet_files: $(TARGET).cpp
	# Here is the "preprocessing".
	# It creates a .cpp file based with the same name as the .pde file.
	# On top of the new .cpp file comes the Arduino.h header.
	# At the end there is a generic main() function attached.
	# Then the .cpp file will be compiled. Errors during compile will
	# refer to this new, automatically generated, file.
	# Not the original .pde file you actually edit...
	test -d applet || mkdir applet
	echo '#include "Arduino.h"' > applet/$(TARGET).cpp
	cat $(TARGET).cpp >> applet/$(TARGET).cpp
	#cat $(ARDUINO_SRC)/main.cpp >> applet/$(TARGET).cpp

elf: applet/$(TARGET).elf
hex: applet/$(TARGET).hex
eep: applet/$(TARGET).eep
lss: applet/$(TARGET).lss
sym: applet/$(TARGET).sym
bin: applet/$(TARGET).bin
s: applet/$(TARGET).s

# Program the device.
upload: applet/$(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)


	# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) applet/$(TARGET).hex
ELFSIZE = $(SIZE)  applet/$(TARGET).elf
sizebefore:
	@if [ -f applet/$(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(HEXSIZE); echo; fi

sizeafter:
	@if [ -f applet/$(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(HEXSIZE); echo; fi


# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
--change-section-address .data-0x800000 \
--change-section-address .bss-0x800000 \
--change-section-address .noinit-0x800000 \
--change-section-address .eeprom-0x810000


coff: applet/$(TARGET).elf
	$(COFFCONVERT) -O coff-avr applet/$(TARGET).elf $(TARGET).cof


extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr applet/$(TARGET).elf $(TARGET).cof


.SUFFIXES: .elf .hex .eep .lss .sym .bin .s

.elf.hex:
	$(OBJCOPY) -O $(FORMAT) -S -R .eeprom $< $@

.elf.eep:
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@

.elf.bin:
	$(OBJCOPY) -O $(FORMAT) applet/$(TARGET).elf applet/$(TARGET).bin

	# Link: create ELF output file from library.
applet/$(TARGET).elf: $(TARGET).cpp applet/libcore.a
	$(CC) $(ALL_CFLAGS) -s -o $@ applet/$(TARGET).cpp -L. -L./applet -lcore $(LDFLAGS)
	avr-strip --strip-all $@

applet/libcore.a: $(OBJ)
	@for i in $(OBJ); do echo $(AR) rcs applet/libcore.a $$i; $(AR) rcs applet/libcore.a $$i; done

# Compile: create object files from C++ source files.
.cpp.o: $(HEADERS)
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@

# Compile: create assembler files from C++ source files.
.cpp.s:
	$(CXX) -S $(ALL_CXXFLAGS) $< -o $@

# Compile: create object files from C source files.
.c.o: $(HEADERS)
	$(CC) -c $(ALL_CFLAGS) $< -o $@

# Compile: create assembler files from C source files.
.c.s:
	$(CC) -S $(ALL_CFLAGS) $< -o $@

# Assemble: create object files from assembler source files.
.S.o:
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

# Target: clean project.
clean:
	$(REMOVE) -- applet/$(TARGET).hex applet/$(TARGET).eep \
	applet/$(TARGET).cof applet/$(TARGET).elf applet/$(TARGET).map \
	applet/$(TARGET).sym applet/$(TARGET).lss applet/$(TARGET).cpp \
	applet/$(TARGET).s  applet/$(TARGET).bin applet/libcore.a \
	$(OBJ) $(LST) $(SRC:.c=.s) $(SRC:.c=.d) $(CXXSRC:.cpp=.s) $(CXXSRC:.cpp=.d)
	rmdir -- applet

depend:
	if grep '^# DO NOT DELETE' $(MAKEFILE) >/dev/null; \
	then \
		sed -e '/^# DO NOT DELETE/,$$d' $(MAKEFILE) > \
			$(MAKEFILE).$$$$ && \
		$(MV) $(MAKEFILE).$$$$ $(MAKEFILE); \
	fi
	echo '# DO NOT DELETE THIS LINE -- make depend depends on it.' \
		>> $(MAKEFILE); \
	$(CC) -M -mmcu=$(MCU) $(CDEFS) $(CINCS) $(SRC) $(ASRC) >> $(MAKEFILE)

.PHONY:	all build elf hex eep lss sym program coff extcoff clean depend applet_files sizebefore sizeafter
