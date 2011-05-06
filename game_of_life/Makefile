CC=gcc
CFLAGS=-Wall -O2 -g -pedantic
CXXFLAGS=$(CFLAGS)

PROGRAM=life
SOURCES=main.c
OBJS=$(SOURCES:.c=.o)

INCLUDES=
LDFLAGS=-lncurses -lpthread

RM=rm -f
INSTALL=install
PREFIX=/usr/local

all:	$(PROGRAM)

life: $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)

distclean:  clean
	$(RM) $(PROGRAM)
