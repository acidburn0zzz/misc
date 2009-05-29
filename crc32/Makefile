CC=gcc
CFLAGS=-Wall -O2 -g -pedantic -ansi -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
CXXFLAGS=$(CFLAGS)

PROGRAM=crc32
SOURCES=main.c sfv.c
OBJS=$(SOURCES:.c=.o)

INCLUDES=-I../libs/libcrc32
LDFLAGS=-L../libs/libcrc32 -lcrc32

RM=rm
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
	$(RM) -f $(OBJS)

distclean:  clean
	$(RM) -f $(PROGRAM)
