CC=gcc
CFLAGS=-Wall -O2 -g -pedantic -ansi -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
CXXFLAGS=$(CFLAGS)

PROGRAM=crc32
SOURCES=main.c
OBJS=$(SOURCES:.c=.o)

INCLUDES=-I../libs/libhash
LDFLAGS=-L../libs/libhash -lhash -lpthread

RM=rm -f
INSTALL=install
PREFIX=/usr/local

all:	$(PROGRAM)

crc32: $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

install:
	$(INSTALL) -D $(PROGRAM) $(PREFIX)/bin/$(PROGRAM)

clean:
	$(RM) $(OBJS)

distclean:  clean
	$(RM) $(PROGRAM)
