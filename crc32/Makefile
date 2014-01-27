CC=gcc
CFLAGS=-g -O2 -pedantic -std=c99 -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_POSIX_C_SOURCE=199309L
CXXFLAGS=$(CFLAGS)

PROGRAM=crc32
SOURCES=main.c
OBJS=$(SOURCES:.c=.o)

INCLUDES=-I../libs/libhash
LDFLAGS=-L../libs/libhash -lhash -lpthread

INSTALL=install
PREFIX=/usr/local

all:	$(PROGRAM)

libhash:
	make -C ../libs/libhash

crc32: $(OBJS) libhash
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

install:
	$(INSTALL) -D $(PROGRAM) $(PREFIX)/bin/

clean:
	$(RM) $(OBJS)

distclean:  clean
	$(RM) $(PROGRAM)
