TARGET = chuck
OBJS = main.o

BUILD_PRX = 1
PSP_FW_VERSION = 550

# Define the name of our custom exports (minus the .exp extension??)
#PRX_EXPORTS=exports.exp

CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
STDLIBS=
LIBS=$(STDLIBS)

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
