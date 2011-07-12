CC=gcc
CFLAGS=-g -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -I../misc
CXXFLAGS=$(CFLAGS)
LDFLAGS=

PROGRAMS=bfint bfdbg

PREFIX=/usr/local

all:	$(PROGRAMS)

bfint:
	$(CC) -o bfint $(CFLAGS) main.c ../misc/hexdump.c $(LDFLAGS)

bfdbg:
	$(CC) -o bfdbg $(CFLAGS) -DDEBUG main.c ../misc/hexdump.c $(LDFLAGS)

install:
	$(INSTALL) -D $(PROGRAMS) $(PREFIX)/bin/$(PROGRAM)

clean:
	$(RM) $(PROGRAMS) *~
