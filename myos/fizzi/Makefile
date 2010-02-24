CC=gcc
CFLAGS=-Wall -O2 -g
CXXFLAGS=$(CFLAGS)

PROGRAM=mios
SOURCES=myOS.c mySH.c myKernel.c
OBJS=$(SOURCES:.c=.o)

INCLUDES=
LDFLAGS=

RM=rm -f

all:	$(PROGRAM)

mios: $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)

distclean:  clean
	$(RM) $(PROGRAM)
