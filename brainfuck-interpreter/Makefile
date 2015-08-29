CC=gcc
CFLAGS=-g -pedantic -std=c99 -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -I../misc
CXXFLAGS=$(CFLAGS)
LDFLAGS=

PROGRAMS=bfint

PREFIX=/usr/local

all:
	$(CC) -o bfint $(CFLAGS) main.c ../misc/hexdump.c $(LDFLAGS)

install:
	$(INSTALL) -D $(PROGRAMS) $(PREFIX)/bin/$(PROGRAM)

clean:
	$(RM) bfint *~
